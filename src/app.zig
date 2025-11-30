const std = @import("std");
const sdl = @import("sdl.zig").sdl;
const bgfx = @import("bgfx.zig").bgfx;
const events = @import("events.zig");
const backend = @import("backend.zig");
const ecs = @import("ecs.zig");

const Event = events.Event;
const EventType = events.EventType;
const Key = events.Key;

const BackendType = backend.BackendType;
const Backend = backend.Backend;

pub fn create(
    title: [:0]const u8,
    width: u32,
    height: u32,
    options: struct { background_color: u32 = 0x303030ff },
) !App {
    return App{
        .backend = try Backend.create(title, width, height),
        .registry = ecs.Registry.create(),
        .background_color = options.background_color,
    };
}

pub const App = struct {
    backend: Backend,
    background_color: u32,
    registry: ecs.Registry,
    running: bool = false,

    pub fn run(self: *App) void {
        self.running = true;

        while (self.running) {
            self.processEvents();
            self.update();
            self.render();
        }
    }

    pub fn dispose(self: *App) void {
        self.backend.dispose();
    }

    fn processEvents(self: *App) void {
        var event: Event = undefined;

        while (self.backend.pollEvent(&event)) {
            std.debug.print(":> Event; type = {}, key = {}\n", .{ event.eventType, event.key });
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
        self.backend.renderer.renderViewColor(self.background_color, .{});

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
