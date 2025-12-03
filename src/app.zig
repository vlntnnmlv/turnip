const std = @import("std");
const sdl = @import("sdl.zig").sdl;
const bgfx = @import("bgfx.zig").bgfx;
const events = @import("events.zig");
const backend = @import("backend.zig");
const assetLoader = @import("assetLoader.zig");

const AssetLoader = assetLoader.AssetLoader;

const Event = events.Event;
const EventType = events.EventType;
const Key = events.Key;

const BackendType = backend.BackendType;
const Backend = backend.Backend;

pub fn init(title: [:0]const u8, width: u32, height: u32) !App {
    return App{
        .backend = try Backend.init(App.allocator, title, width, height),
        // .registry = ecs.Registry.create(),
        // .background_color = options.background_color,
    };
}

pub const App = struct {
    backend: Backend,
    running: bool = false,
    // background_color: u32,
    // registry: ecs.Registry,

    var allocator: std.mem.Allocator = std.heap.c_allocator;
    // var img: bgfx.bgfx_texture_handle_t = undefined;

    pub fn init(title: [:0]const u8, width: u32, height: u32) !App {
        return App{
            .backend = try Backend.init(allocator, title, width, height),
            // .registry = ecs.Registry.create(),
            // .background_color = options.background_color,
        };
    }

    pub fn deinit(self: *App) void {
        self.backend.deinit();
    }

    pub fn run(self: *App) void {
        // img = AssetLoader.loadTexture("bean") catch unreachable;
        self.running = true;

        while (self.running) {
            self.processEvents();
            self.update();
            self.render();
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

    fn render(self: *App) void {
        self.backend.renderer.beginRender();
        self.backend.renderer.renderViewColor(0xffffffff, .{});

        // self.backend.renderer.renderTexture(.{ .idx = img.idx }, .{ .x = 0, .y = 0, .width = 100, .height = 100 });
        // bgfx.bgfx_set_debug(bgfx.BGFX_DEBUG_TEXT);
        // bgfx.bgfx_dbg_text_clear(0, false);
        // bgfx.bgfx_dbg_text_printf(1, 1, 0x4f, "Entities count: %d", registry.alive.items.len);

        // var it = registry.components.iterator();
        // var i: u16 = 2;
        // while (it.next()) |entry| {
        //     bgfx.bgfx_dbg_text_printf(1, i, 0x4f, "Component %s : %d", entry.key_ptr.name.ptr, entry.value_ptr.len);
        //     i += 1;
        // }

        self.backend.renderer.endRender();
    }
};
