const std = @import("std");
const sdl = @import("sdl.zig").sdl;
const bgfx = @import("bgfx.zig").bgfx;
const ecs = @import("ecs.zig");
const events = @import("events.zig");
const backend = @import("backend.zig");
const assetLoader = @import("assetLoader.zig");
const scenes = @import("scenes.zig");

const List = std.ArrayList;

const AssetLoader = assetLoader.AssetLoader;

const Event = events.Event;
const EventType = events.EventType;
const Key = events.Key;

const BackendType = backend.BackendType;
const Backend = backend.Backend;

const Scene = scenes.Scene;

pub const App = struct {
    allocator: std.mem.Allocator,
    backend: Backend,
    scenes: List(Scene) = .empty,
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

    pub fn addScene(
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
            self.processEvents();
            self.update();
            try self.render();
        }
    }

    fn processEvents(self: *App) void {
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
        // for (self.scenes.items) |scene| {
        //     const cameras = try scene.registry.with(ecs.Camera);
        //     const camera: *ecs.Camera = scene.registry.getComponent(cameras.items[0], ecs.Camera) orelse continue;

        // }

        self.backend.renderer.setCamera(ecs.Camera{
            .view_type = ecs.Camera.ViewType.ORTHOGONAL,
            .options = ecs.Camera.Options{ .width = self.backend.width, .height = self.backend.height },
        });

        self.backend.renderer.beginRender();

        for (self.scenes.items) |scene| {
            try self.renderScene(scene);
        }

        // self.backend.renderer.fill(0x444444ff, .{});

        // bgfx.bgfx_set_debug(bgfx.BGFX_DEBUG_TEXT);
        // bgfx.bgfx_dbg_text_clear(0, false);
        // bgfx.bgfx_dbg_text_printf(1, 1, 0x4f, "Entities count");
        self.backend.renderer.endRender();
    }

    fn renderScene(self: *App, scene: Scene) !void {
        const cameras = scene.registry.with(ecs.Camera) catch return;
        if (cameras.items.len == 0) return;
        const camera_entity = cameras.items[0];
        const camera = scene.registry.getComponent(camera_entity, ecs.Camera) orelse return;

        self.backend.renderer.setCamera(camera.*);
        self.backend.renderer.fill(scene.options.background_color, .{});

        const sprites = scene.registry.with(ecs.Sprite) catch return;
        if (sprites.items.len == 0) return;
        for (sprites.items) |sprite_entity| {
            const transform = scene.registry.getComponent(sprite_entity, ecs.Transform2D) orelse continue;
            const sprite = scene.registry.getComponent(sprite_entity, ecs.Sprite) orelse continue;
            try self.backend.renderer.renderTexture(
                bgfx.bgfx_texture_handle_t{ .idx = sprite.texture_reference.idx },
                transform.rectangle,
            );
        }
    }
};
