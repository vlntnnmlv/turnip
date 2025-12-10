const std = @import("std");
const ecs = @import("ecs.zig");

const Registry = ecs.Registry;
const System = @import("systems.zig").System;
const NamedComponentsTuple = @import("components_meta.zig").NamedComponentsTuple;

pub const Scene = struct {
    pub const Options = struct { background_color: u32 = 0xFFFFFFFF };

    allocator: std.mem.Allocator,
    name: []const u8,
    options: Options,
    registry: Registry,
    systems: std.ArrayList(System) = .empty,

    pub fn init(
        allocator: std.mem.Allocator,
        name: []const u8,
        options: Options,
    ) Scene {
        return Scene{
            .name = name,
            .allocator = allocator,
            .registry = Registry.init(allocator),
            .options = options,
        };
    }

    pub fn deinit(self: *Scene) void {
        self.registry.deinit();
    }

    pub fn addSystem(
        self: *Scene,
        comptime ComponentTypes: anytype,
        comptime run_function: fn (NamedComponentsTuple(ComponentTypes)) void,
    ) !void {
        const Closure = struct {
            registry: Registry,

            fn run(this: *@This()) void {
                var iter = this.registry.view(ComponentTypes);
                defer iter.deinit();

                while (iter.next()) |view| {
                    run_function(view);
                }
            }
        };

        const closure = try self.allocator.create(Closure);
        closure.* = .{
            .registry = self.registry,
        };

        const system = System.create(closure, Closure.run);
        try self.systems.append(self.allocator, system);
    }

    pub fn update(self: *Scene) void {
        for (self.systems.items) |system| {
            system.run();
        }
    }
};
