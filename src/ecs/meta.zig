const std = @import("std");
const builtin = @import("std").builtin;

const EntityID = @import("entity.zig").EntityID;
const Registry = @import("registry.zig").Registry;

pub const ComponentDescription = struct {
    name: [:0]const u8,
    size: usize,
    alignment: u8,

    pub fn create(comptime T: type) ComponentDescription {
        return ComponentDescription{
            .name = @typeName(T),
            .size = @sizeOf(T),
            .alignment = @alignOf(T),
        };
    }
};

pub const ComponentDescriptionContext = struct {
    pub fn hash(_: *const ComponentDescriptionContext, value: ComponentDescription) u32 {
        var hasher = std.hash.XxHash32.init(0);
        _ = hasher.update(value.name);
        _ = hasher.update(std.mem.asBytes(&value.size));
        _ = hasher.update(std.mem.asBytes(&value.alignment));
        return hasher.final();
    }

    pub fn eql(_: *const ComponentDescriptionContext, left: ComponentDescription, right: ComponentDescription, _: usize) bool {
        return std.mem.eql(u8, left.name, right.name) and
            left.size == right.size and
            left.alignment == right.alignment;
    }
};

pub const ComponentBucket = struct {
    const Map = std.AutoHashMap;
    const List = std.ArrayList;

    allocator: std.mem.Allocator,
    size: usize,
    alignment: u8,
    components: Map(EntityID, []u8),
    len: u32 = 0,

    pub fn init(allocator: std.mem.Allocator, comptime T: type) ComponentBucket {
        return ComponentBucket{
            .allocator = allocator,
            .size = @sizeOf(T),
            .alignment = @alignOf(T),
            .components = Map(EntityID, []u8).init(allocator),
        };
    }

    pub fn deinit(self: *ComponentBucket) void {
        var components_iterator = self.components.valueIterator();
        while (components_iterator.next()) |component| {
            self.allocator.free(component.*);
        }
        self.components.deinit();
    }

    pub fn set(self: *ComponentBucket, entity_id: EntityID, comptime T: type, value: T) !void {
        if (@sizeOf(T) != self.size or @alignOf(T) != self.alignment) {
            return error.MismatchedLayout;
        }

        // TODO: Do not reallocate, just put new value in allocated memory
        if (self.components.contains(entity_id)) {
            const mem = self.components.getPtr(entity_id).?;
            self.allocator.free(mem.*);
        }

        const mem_raw = try self.allocator.alignedAlloc(
            u8,
            std.mem.Alignment.fromByteUnits(@alignOf(T)),
            @sizeOf(T),
        );

        const mem: []u8 = @as([]u8, @ptrCast(mem_raw));
        @memmove(mem, std.mem.asBytes(&value));

        try self.components.put(entity_id, mem_raw);
        self.len += 1;
    }

    pub fn get(self: *ComponentBucket, entityID: EntityID, comptime T: type) ?*T {
        const mem: []u8 = self.components.get(entityID) orelse return null;

        const raw_ptr = mem.ptr;
        const aligned_ptr = @as(*T, @ptrCast(@alignCast(raw_ptr)));
        return aligned_ptr;
    }

    pub fn remove(self: *ComponentBucket, entityID: EntityID) bool {
        const mem: []u8 = self.components.get(entityID) orelse return false;

        self.allocator.free(mem);
        return self.components.remove(entityID);
    }
};

fn type2FieldName(comptime ComponentType: type) [:0]const u8 {
    const full_name = @typeName(ComponentType);
    const dot_index = std.mem.lastIndexOfScalar(u8, full_name, '.').? + 1;
    const name = full_name[dot_index..];

    var lower_name: [name.len:0]u8 = undefined;
    var lower_name_len: usize = 0;

    inline for (name, 0..) |c, j| {
        lower_name[j] = std.ascii.toLower(c);
        lower_name_len += 1;
    }
    lower_name[name.len] = 0;
    return lower_name[0..lower_name_len :0];
}

pub fn NamedComponentsTuple(comptime ComponentTypes: anytype) type {
    var fields: [ComponentTypes.len]builtin.Type.StructField = undefined;
    for (0..ComponentTypes.len) |i| {
        fields[i] =
            .{
                .name = type2FieldName(ComponentTypes[i]),
                .type = *ComponentTypes[i],
                .default_value_ptr = null,
                .is_comptime = false,
                .alignment = @alignOf(ComponentTypes[i]),
            };
    }

    return @Type(.{
        .@"struct" = .{
            .layout = .auto,
            .fields = &fields,
            .decls = &[_]builtin.Type.Declaration{},
            .is_tuple = false,
        },
    });
}

pub fn ComponentsViewIterator(comptime ComponentTypes: anytype) type {
    comptime {
        return struct {
            const Self = @This();
            pub const ComponentsViewType = NamedComponentsTuple(ComponentTypes);

            registry: *const Registry,
            current_entity_index: usize = 0,
            matching_entities: std.ArrayList(EntityID) = .empty,
            allocator: std.mem.Allocator,

            pub fn init(registry: *const Registry) !Self {
                var first_entities = try registry.with(ComponentTypes[0]);
                defer first_entities.deinit(registry.allocator);

                var filtered: std.ArrayList(EntityID) = .empty;

                for (first_entities.items) |entity_id| {
                    var has_all_components = true;
                    inline for (1..ComponentTypes.len) |component_index| {
                        const ComponentType = ComponentTypes[component_index];
                        const component_description = ComponentDescription.create(ComponentType);
                        if (!registry.components.contains(component_description) or
                            !registry.components.getPtr(component_description).?.components.contains(entity_id))
                        {
                            has_all_components = false;
                            break;
                        }
                    }

                    if (has_all_components) {
                        try filtered.append(registry.allocator, entity_id);
                    }
                }

                return Self{
                    .registry = registry,
                    .matching_entities = filtered,
                    .allocator = registry.allocator,
                };
            }

            pub fn deinit(self: *Self) void {
                self.matching_entities.deinit(self.allocator);
            }

            pub fn reset(self: *Self) void {
                self.current_entity_index = 0;
            }

            pub fn next(self: *Self) ?ComponentsViewType {
                if (self.current_entity_index >= self.matching_entities.items.len) {
                    return null;
                }

                const entity = self.matching_entities.items[self.current_entity_index];
                self.current_entity_index += 1;

                var view_item: ComponentsViewType = undefined;
                inline for (ComponentTypes) |ComponentType| {
                    const desc = ComponentDescription.create(ComponentType);
                    const bucket = self.registry.components.getPtr(desc).?;
                    const component_ptr = bucket.get(entity, ComponentType).?;

                    const field_name = comptime type2FieldName(ComponentType);
                    @field(view_item, field_name) = component_ptr;
                }

                return view_item;
            }
        };
    }
}
