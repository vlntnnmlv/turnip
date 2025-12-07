const std = @import("std");
const sdl = @import("sdl.zig").sdl;
const bgfx = @import("bgfx.zig").bgfx;
const ecs = @import("ecs.zig");
const events = @import("events.zig");
const backend = @import("backend.zig");
const assetLoader = @import("asset_loader.zig");
const scenes = @import("scenes.zig");

const components = @import("components.zig");
const Camera = components.Camera;
const Transform2D = components.Transform2D;
const Sprite = components.Sprite;

const AssetLoader = assetLoader.AssetLoader;

const Event = events.Event;
const EventType = events.EventType;
const Key = events.Key;

const BackendType = backend.BackendType;
const Backend = backend.Backend;

const Scene = scenes.Scene;

// TODO: Add systems
pub const App = struct {
    allocator: std.mem.Allocator,
    backend: Backend,
    scenes: std.ArrayList(Scene) = .empty,
    running: bool = false,

    pub fn init(allocator: std.mem.Allocator, title: [:0]const u8, width: u32, height: u32) !App {
        return App{
            .allocator = allocator,
            .backend = try Backend.init(allocator, title, width, height),
        };
    }

    pub fn deinit(self: *App) void {
        self.backend.deinit();
    }

    pub fn add(
        self: *App,
        name: [:0]const u8,
        options: Scene.Options,
    ) !*Scene {
        try self.scenes.append(self.allocator, Scene.init(self.allocator, name, options));
        return &self.scenes.items[self.scenes.items.len - 1];
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
        }
    }

    fn update(self: *App) void {
        _ = self;
    }

    fn render(self: *App) !void {
        self.backend.renderer.beginRender();

        for (self.scenes.items) |scene| {
            try self.renderScene(scene);
        }

        self.backend.renderer.endRender();
    }

    // TODO: Add mutiple cameras rendering, this will just render the first one.
    fn renderScene(self: *App, scene: Scene) !void {
        const cameras = scene.registry.with(Camera) catch return;
        for (cameras.items) |camera_entity| {
            const camera = scene.registry.get(camera_entity, Camera) orelse continue;

            self.backend.renderer.setCamera(camera.*);
            self.backend.renderer.fill(scene.options.background_color, .{});

            const sprites = scene.registry.with(Sprite) catch return;
            if (sprites.items.len == 0) return;
            for (sprites.items) |sprite_entity| {
                const transform = scene.registry.get(sprite_entity, Transform2D) orelse continue;
                const sprite = scene.registry.get(sprite_entity, Sprite) orelse continue;
                try self.backend.renderer.renderTexture(
                    bgfx.bgfx_texture_handle_t{ .idx = sprite.texture_reference.idx },
                    transform.rectangle,
                );
            }
        }
    }
};
