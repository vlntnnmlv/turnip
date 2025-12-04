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

    pub fn addScene(self: *App, name: [:0]const u8) !*Scene {
        try self.scenes.append(self.allocator, Scene.init(self.allocator, name));
        return &self.scenes.items[self.scenes.items.len - 1];
    }

    var img: bgfx.bgfx_texture_handle_t = undefined;

    pub fn run(self: *App) !void {
        img = AssetLoader.loadTexture("devil") catch unreachable;
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

        self.backend.renderer.fill(0x444444ff, .{});

        const cells = 16.0;
        const cell_size: f32 = @as(f32, @floatFromInt(self.backend.width)) / cells;
        for (0..@as(u8, @intFromFloat(cells))) |i| {
            for (0..@as(u8, @intFromFloat(cells))) |j| {
                try self.backend.renderer.renderTexture(.{ .idx = img.idx }, .{
                    .x = @as(f32, @floatFromInt(i)) * cell_size,
                    .y = @as(f32, @floatFromInt(j)) * cell_size,
                    .width = cell_size,
                    .height = cell_size,
                });
            }
        }

        // bgfx.bgfx_set_debug(bgfx.BGFX_DEBUG_TEXT);
        // bgfx.bgfx_dbg_text_clear(0, false);
        // bgfx.bgfx_dbg_text_printf(1, 1, 0x4f, "Entities count");
        self.backend.renderer.endRender();

        // var it = registry.components.iterator();
        // var i: u16 = 2;
        // while (it.next()) |entry| {
        //     bgfx.bgfx_dbg_text_printf(1, i, 0x4f, "Component %s : %d", entry.key_ptr.name.ptr, entry.value_ptr.len);
        //     i += 1;
        // }

    }
};
