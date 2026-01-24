const std = @import("std");
const sdl = @import("sdl.zig").sdl;
const bgfx = @import("bgfx.zig").bgfx;
const events = @import("events.zig");
const backend = @import("backend.zig");
const asset_loader = @import("asset_loader.zig");
const asset_manager = @import("asset_manager.zig");
const components = @import("components.zig");

const Camera = components.Camera;
const Transform2D = components.Transform2D;
const Sprite = components.Sprite;
const Mesh = components.Mesh;

const AssetLoader = asset_loader.AssetLoader;
const AssetManager = asset_manager.AssetManager;

const Event = events.Event;
const EventType = events.EventType;
const Key = events.Key;

const BackendType = backend.BackendType;
const Backend = backend.Backend;

const World = @import("fey_ecs").World;

pub const App = struct {
    allocator: std.mem.Allocator,
    backend: Backend,
    asset_manager: AssetManager,
    worlds: std.StringHashMap(World),
    running: bool = false,
    events_q: std.ArrayList(Event) = .empty,

    const Self = @This();

    pub fn init(allocator: std.mem.Allocator, title: [:0]const u8, width: u32, height: u32) !Self {
        return Self{
            .allocator = allocator,
            .backend = try Backend.init(allocator, title, width, height),
            .asset_manager = AssetManager.init(allocator),
            .worlds = std.StringHashMap(World).init(allocator),
        };
    }

    pub fn deinit(self: *Self) void {
        var worlds_iterator = self.worlds.valueIterator();
        while (worlds_iterator.next()) |world| {
            world.deinit();
        }

        self.worlds.deinit();
        self.asset_manager.deinit();
        self.backend.deinit();
    }

    pub fn add(
        self: *Self,
        name: []const u8,
    ) !*World {
        try self.worlds.put(name, try World.init(self.allocator));
        if (self.worlds.getPtr(name)) |world| {
            return world;
        } else unreachable;
    }

    pub fn run(self: *Self) !void {
        self.running = true;

        while (self.running) {
            self.process();
            try self.update();
            try self.render();
        }
    }

    fn process(self: *Self) void {
        var event: Event = undefined;
        while (self.backend.pollEvent(&event)) {
            switch (event.eventType) {
                EventType.Quit => self.running = false,
                EventType.KeyPressed => {
                    if (event.key == Key.ESCAPE)
                        self.running = false;
                },
                else => {},
            }

            self.events_q.append(self.allocator, event) catch {
                unreachable;
            };

            // TODO: Move this to an input system.
            // var scenes_iterator = self.worlds.valueIterator();
            // while (scenes_iterator.next()) |scene| {
            //     for (scene.event_callbacks.items) |callback| {
            //         callback(scene, event);
            //     }
            // }
        }
    }

    fn update(self: *Self) !void {
        var scenes_iterator = self.worlds.valueIterator();
        while (scenes_iterator.next()) |scene| {
            try scene.update();
        }
    }

    fn render(self: *Self) !void {
        self.backend.renderer.beginRender();

        var scenes_iterator = self.worlds.valueIterator();
        while (scenes_iterator.next()) |scene| {
            try self.renderScene(scene);
        }

        self.backend.renderer.endRender();
    }

    // TODO: Add mutiple cameras rendering, this will just render the first one.
    fn renderScene(self: *App, scene: *World) !void {
        var cameras = scene.registry.view(.{Camera});
        defer cameras.deinit();
        while (cameras.next()) |camera_view| {
            const camera = camera_view.camera;

            self.backend.renderer.setCamera(camera.*);

            // TODO:
            // self.backend.renderer.fill(scene.options.background_color, .{});
            self.backend.renderer.fill(0x424242ff, .{});

            var to_render = scene.registry.view(.{ Sprite, Transform2D });
            while (to_render.next()) |render_view| {
                try self.backend.renderer.renderTexture(
                    bgfx.bgfx_texture_handle_t{
                        .idx = render_view.sprite.texture_reference.id,
                    },
                    render_view.transform2d.rectangle,
                );
            }

            var to_render_mesh = scene.registry.view(.{ Sprite, Mesh });
            while (to_render_mesh.next()) |render_view| {
                try self.backend.renderer.renderMesh(
                    bgfx.bgfx_texture_handle_t{
                        .idx = render_view.sprite.texture_reference.id,
                    },
                    render_view.mesh.*,
                );
            }
        }
    }
};
