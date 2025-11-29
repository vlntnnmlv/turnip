const std = @import("std");
const sdl = @import("sdl.zig").sdl;
const bgfx = @import("bgfx.zig").bgfx;
const events = @import("events.zig");
const backend_m = @import("backend.zig");

const Event = events.Event;
const EventType = events.EventType;
const Key = events.Key;

const BackendType = backend_m.BackendType;
const Backend = backend_m.Backend;

pub fn create(
    title: [:0]const u8,
    width: u32,
    height: u32,
    options: struct { backendType: BackendType = BackendType.SDL_BGFX, backgroundColor: u32 = 0x303030ff },
) !Window {
    const backend = try Backend.create(options.backendType, title, width, height);
    return Window{ .backend = backend, .backgroundColor = options.backgroundColor };
}

pub const Window = struct {
    backend: Backend,
    running: bool = false,
    backgroundColor: u32,

    pub fn run(self: *Window) void {
        self.running = true;
        while (self.running) {
            self.processEvents();
            self.update();
            self.render();
        }
    }

    fn processEvents(self: *Window) void {
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

    fn update(self: *Window) void {
        _ = self;
    }

    fn render(self: *Window) void {
        bgfx.bgfx_touch(0);

        bgfx.bgfx_set_view_clear(0, bgfx.BGFX_CLEAR_COLOR | bgfx.BGFX_CLEAR_DEPTH, self.backgroundColor, 0, 0);

        bgfx.bgfx_set_debug(bgfx.BGFX_DEBUG_TEXT);
        bgfx.bgfx_dbg_text_clear(0, false);
        // bgfx.bgfx_dbg_text_printf(1, 1, 0x4f, "Entities count: %d", registry.alive.items.len);

        // var it = registry.components.iterator();
        // var i: u16 = 2;
        // while (it.next()) |entry| {
        //     bgfx.bgfx_dbg_text_printf(1, i, 0x4f, "Component %s : %d", entry.key_ptr.name.ptr, entry.value_ptr.len);
        //     i += 1;
        // }

        _ = bgfx.bgfx_frame(false);
    }
};
