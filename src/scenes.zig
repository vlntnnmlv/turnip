const std = @import("std");
const ecs = @import("ecs.zig");

pub const Scene = struct {
    allocator: std.mem.Allocator,
    name: [:0]const u8,
    registry: ecs.Registry,

    pub fn init(allocator: std.mem.Allocator, name: [:0]const u8) Scene {
        return Scene{
            .name = name,
            .allocator = allocator,
            .registry = ecs.Registry.init(allocator),
        };
    }
};
