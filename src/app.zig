const std = @import("std");
const sdl = @import("sdl.zig").sdl;
const bgfx = @import("bgfx.zig").bgfx;
const ecs = @import("ecs.zig");
const events = @import("events.zig");
const backend = @import("backend.zig");
const asset_loader = @import("asset_loader.zig");
const asset_manager = @import("asset_manager.zig");
const scenes = @import("scenes.zig");

const components = ecs.components;
const Camera = components.Camera;
const Transform2D = components.Transform2D;
const Sprite = components.Sprite;

const AssetLoader = asset_loader.AssetLoader;
const AssetManager = asset_manager.AssetManager;

const Event = events.Event;
const EventType = events.EventType;
const Key = events.Key;

const BackendType = backend.BackendType;
const Backend = backend.Backend;

const Scene = scenes.Scene;

pub const App = struct {
    allocator: std.mem.Allocator,
    backend: Backend,
    asset_manager: AssetManager,
    scenes: std.StringHashMap(Scene),
    running: bool = false,

    pub fn init(allocator: std.mem.Allocator, title: [:0]const u8, width: u32, height: u32) !App {
        return App{
            .allocator = allocator,
            .backend = try Backend.init(allocator, title, width, height),
            .asset_manager = AssetManager.init(allocator),
            .scenes = std.StringHashMap(Scene).init(allocator),
        };
    }

    pub fn deinit(self: *App) void {
        var scenes_iterator = self.scenes.valueIterator();
        while (scenes_iterator.next()) |scene| {
            scene.deinit();
        }

        self.scenes.deinit();
        self.asset_manager.deinit();
        self.backend.deinit();
    }

    pub fn add(
        self: *App,
        name: []const u8,
        options: Scene.Options,
    ) !*Scene {
        try self.scenes.put(name, Scene.init(self.allocator, name, options));
        if (self.scenes.getPtr(name)) |scene| {
            return scene;
        } else unreachable;
    }

    pub fn run(self: *App) !void {
        self.running = true;

        while (self.running) {
            self.process();
            self.update();
            try self.render();
        }
    }

    fn process(self: *App) void {
        var event: Event = undefined;
        while (self.backend.pollEvent(&event)) {
            if (event.eventType == EventType.Quit)
                self.running = false;

            if (event.eventType == EventType.KeyPressed) {
                if (event.key == Key.ESCAPE)
                    self.running = false;
            }

            var scenes_iterator = self.scenes.valueIterator();
            while (scenes_iterator.next()) |scene| {
                for (scene.event_callbacks.items) |callback| {
                    callback(scene, event);
                }
            }
        }
    }

    fn update(self: *App) void {
        var scenes_iterator = self.scenes.valueIterator();
        while (scenes_iterator.next()) |scene| {
            scene.update();
        }
    }

    fn render(self: *App) !void {
        self.backend.renderer.beginRender();

        var scenes_iterator = self.scenes.valueIterator();
        while (scenes_iterator.next()) |scene| {
            try self.renderScene(scene);
        }

        self.backend.renderer.endRender();
    }

    // TODO: Add mutiple cameras rendering, this will just render the first one.
    fn renderScene(self: *App, scene: *Scene) !void {
        var cameras = scene.registry.view(.{Camera});
        defer cameras.deinit();
        while (cameras.next()) |camera_view| {
            const camera = camera_view.camera;

            self.backend.renderer.setCamera(camera.*);
            self.backend.renderer.fill(scene.options.background_color, .{});

            var to_render = scene.registry.view(.{ Sprite, Transform2D });
            while (to_render.next()) |render_view| {
                try self.backend.renderer.renderTexture(
                    bgfx.bgfx_texture_handle_t{
                        .idx = render_view.sprite.texture_reference.id,
                    },
                    render_view.transform2d.rectangle,
                );
            }
        }
    }
};
