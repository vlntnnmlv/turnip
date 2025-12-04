const std = @import("std");
const ecs = @import("ecs.zig");

pub const Scene = struct {
    pub const Options = struct { background_color: u32 = 0xFFFFFFFF };

    allocator: std.mem.Allocator,
    name: [:0]const u8,
    options: Options,

    registry: ecs.Registry,

    pub fn init(
        allocator: std.mem.Allocator,
        name: [:0]const u8,
        options: Options,
    ) Scene {
        return Scene{
            .name = name,
            .allocator = allocator,
            .registry = ecs.Registry.init(allocator),
            .options = options,
        };
    }
};
