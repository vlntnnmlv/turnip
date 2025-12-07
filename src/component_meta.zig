const std = @import("std");

const EntityID = @import("ecs.zig").EntityID;

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
        self.components.deinit();
    }

    pub fn set(self: *ComponentBucket, entityID: EntityID, comptime T: type, value: T) !void {
        if (@sizeOf(T) != self.size or @alignOf(T) != self.alignment) {
            return error.MismatchedLayout;
        }

        const mem = try self.allocator.alignedAlloc(
            u8,
            std.mem.Alignment.fromByteUnits(@alignOf(T)),
            @sizeOf(T),
        );

        const mem2: []u8 = @as([]u8, @ptrCast(mem));
        @memmove(mem2, std.mem.asBytes(&value));

        try self.components.put(entityID, mem2);
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
