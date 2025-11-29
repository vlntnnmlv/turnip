const std = @import("std");

pub const EntityID = u16;
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

const ComponentDescriptionContext = struct {
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

const Map = std.AutoHashMap;
const List = std.ArrayList;

const ComponentBucket = struct {
    allocator: std.mem.Allocator,
    size: usize,
    alignment: u8,
    components: Map(EntityID, []u8),
    len: u32 = 0,

    pub fn create(allocator: std.mem.Allocator, comptime T: type) ComponentBucket {
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
        const mem: []u8 = self.components.get(entityID) orelse {
            return null;
        };
        const raw_ptr = mem.ptr;
        const aligned_ptr = @as(*T, @ptrCast(@alignCast(raw_ptr)));
        return aligned_ptr;
    }
};

const ComponentsMap = std.ArrayHashMap(
    ComponentDescription,
    ComponentBucket,
    ComponentDescriptionContext,
    true,
);
pub const Registry = struct {
    alive: std.ArrayList(EntityID),
    components: ComponentsMap,

    var allocator = std.heap.c_allocator;
    var nextEntityID: EntityID = 0;

    pub fn create() Registry {
        const registry = Registry{
            .alive = .empty,
            .components = ComponentsMap.init(allocator),
        };
        return registry;
    }

    pub fn createEntity(self: *Registry) !EntityID {
        nextEntityID += 1;
        try self.alive.append(allocator, nextEntityID);

        return nextEntityID;
    }

    pub fn removeEntity(self: *Registry, entityID: EntityID) void {
        var i: usize = 0;
        while (i < self.alive.items.len) {
            if (self.alive.items[i] == entityID) {
                self.alive.orderedRemove(i);
                break;
            }
            i += 1;
        }
        // TODO: Remove assosiated components
    }

    pub fn addComponent(self: *Registry, entityID: EntityID, comptime T: type, value: T) !void {
        const componentDescription: ComponentDescription = ComponentDescription.create(T);
        if (!self.components.contains(componentDescription)) {
            try self.components.put(componentDescription, ComponentBucket.create(allocator, T));
        }

        var componentBucket = self.components.getPtr(componentDescription).?;
        try componentBucket.set(entityID, T, value);
    }

    pub fn getComponent(self: *Registry, entityID: EntityID, comptime T: type) ?*T {
        const componentDescription: ComponentDescription = ComponentDescription.create(T);
        if (!self.components.contains(componentDescription)) return null;

        var componentBucket = self.components.getPtr(componentDescription) orelse return null;
        return componentBucket.get(entityID, T);
    }

    pub fn with(self: *Registry, comptime T: type) !List(EntityID) {
        const componentDescription: ComponentDescription = ComponentDescription.create(T);
        var entityIDs: List(EntityID) = .empty;
        if (!self.components.contains(componentDescription)) return entityIDs;

        const componentBucket = self.components.getPtr(componentDescription).?;
        var it = componentBucket.components.keyIterator();
        while (it.next()) |entityID| {
            try entityIDs.append(allocator, entityID.*);
        }
        return entityIDs;
    }

    pub fn deinit(self: *Registry) void {
        self.alive.deinit(allocator);
        self.components.deinit();
    }
};

pub const Position = struct { x: f32 = 0, y: f32 = 0 };
pub const Character = struct { id: u32 };
pub const Boid = struct { k: f32 };
