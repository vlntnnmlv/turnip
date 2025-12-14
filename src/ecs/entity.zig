pub const Registry = @import("registry.zig").Registry;

pub const EntityID = u16;
pub const Entity = struct {
    entity_id: EntityID,
    registry: *Registry,

    pub fn create(entity_id: EntityID, registry: *Registry) Entity {
        return Entity{
            .entity_id = entity_id,
            .registry = registry,
        };
    }

    pub fn add(self: Entity, comptime T: type, value: T) !void {
        try self.registry.add(self.entity_id, T, value);
    }

    pub fn get(self: Entity, comptime T: type) ?*T {
        return self.registry.get(self.entity_id, T);
    }
};
