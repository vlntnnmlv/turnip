const std = @import("std");

const entity = @import("entity.zig");
const meta = @import("meta.zig");

const EntityID = entity.EntityID;
const Entity = entity.Entity;

const ComponentDescription = meta.ComponentDescription;
const ComponentBucket = meta.ComponentBucket;
const ComponentDescriptionContext = meta.ComponentDescriptionContext;
const ComponentsViewIterator = meta.ComponentsViewIterator;

const ComponentsMap = std.ArrayHashMap(
    ComponentDescription,
    ComponentBucket,
    ComponentDescriptionContext,
    true,
);

pub const Registry = struct {
    allocator: std.mem.Allocator,
    alive: std.ArrayList(EntityID),
    components: ComponentsMap,

    var next_entity_id: EntityID = 0;

    pub fn init(allocator: std.mem.Allocator) Registry {
        const registry = Registry{
            .allocator = allocator,
            .alive = .empty,
            .components = ComponentsMap.init(allocator),
        };
        return registry;
    }

    pub fn deinit(self: *Registry) void {
        self.alive.deinit(self.allocator);

        var component_buckets = self.components.values();
        for (0..component_buckets.len) |i| {
            component_buckets[i].deinit();
        }

        self.components.deinit();
    }

    pub fn create(self: *Registry) !Entity {
        next_entity_id += 1;
        try self.alive.append(self.allocator, next_entity_id);

        return Entity.create(next_entity_id, self);
    }

    pub fn destroy(self: *Registry, entityID: EntityID) void {
        var i: usize = 0;
        while (i < self.alive.items.len) {
            if (self.alive.items[i] == entityID) {
                _ = self.alive.orderedRemove(i);
                break;
            }
            i += 1;
        }

        var components_iterator = self.components.iterator();
        while (components_iterator.next()) |entry| {
            _ = entry.value_ptr.*.remove(entityID);
        }
    }

    pub fn add(self: *Registry, entityID: EntityID, comptime T: type, value: T) !void {
        const componentDescription: ComponentDescription = ComponentDescription.create(T);
        if (!self.components.contains(componentDescription)) {
            try self.components.put(componentDescription, ComponentBucket.init(self.allocator, T));
        }

        var componentBucket = self.components.getPtr(componentDescription).?;
        try componentBucket.set(entityID, T, value);
    }

    pub fn get(self: *const Registry, entityID: EntityID, comptime T: type) ?*T {
        const componentDescription: ComponentDescription = ComponentDescription.create(T);
        if (!self.components.contains(componentDescription)) return null;

        var componentBucket = self.components.getPtr(componentDescription) orelse return null;
        return componentBucket.get(entityID, T);
    }

    pub fn with(self: *const Registry, comptime T: type) !std.ArrayList(EntityID) {
        const componentDescription: ComponentDescription = ComponentDescription.create(T);
        var entityIDs: std.ArrayList(EntityID) = .empty;
        if (!self.components.contains(componentDescription)) return entityIDs;

        const componentBucket = self.components.getPtr(componentDescription).?;
        var it = componentBucket.components.keyIterator();
        while (it.next()) |entityID| {
            try entityIDs.append(self.allocator, entityID.*);
        }
        return entityIDs;
    }

    pub fn view(self: *const Registry, comptime ComponentTypes: anytype) ComponentsViewIterator(ComponentTypes) {
        return ComponentsViewIterator(ComponentTypes).init(self) catch {
            return ComponentsViewIterator(ComponentTypes){
                .registry = self,
                .matching_entities = .empty,
                .allocator = self.allocator,
            };
        };
    }
};
