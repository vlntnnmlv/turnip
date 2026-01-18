const std = @import("std");
const ecs = @import("ecs.zig");
const events = @import("events.zig");

const Registry = ecs.registry.Registry;
const System = ecs.systems.System;
const ComponentsView = ecs.meta.ComponentsView;

const Event = events.Event;

pub const Scene = struct {
    const EventCallback = *const fn (*Scene, Event) void;
    pub const Options = struct { background_color: u32 = 0xFFFFFFFF };

    allocator: std.mem.Allocator,
    name: []const u8,
    options: Options,
    registry: Registry,
    systems: std.ArrayList(System) = .empty,
    event_callbacks: std.ArrayList(EventCallback) = .empty,

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

        self.systems.deinit(self.allocator);
        self.event_callbacks.deinit(self.allocator);
    }

    pub fn addSystem(
        self: *Scene,
        comptime ComponentTypes: anytype,
        comptime run_function: fn (ComponentsView(ComponentTypes)) void,
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

        // TODO: Free the closurea allocation somewhere
        const closure = try self.allocator.create(Closure);
        closure.* = .{
            .registry = self.registry,
        };

        const system = System.create(closure, Closure.run);
        try self.systems.append(self.allocator, system);
    }

    pub fn addEventCallback(self: *Scene, callback: EventCallback) !void {
        try self.event_callbacks.append(self.allocator, callback);
    }

    pub fn update(self: *Scene) void {
        for (self.systems.items) |system| {
            system.run();
        }
    }
};
