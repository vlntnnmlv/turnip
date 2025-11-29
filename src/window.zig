const std = @import("std");
const sdl = @import("sdl.zig").sdl;
const bgfx = @cImport(@cInclude("bgfx/c99/bgfx.h"));
const events = @import("events.zig");

const Event = events.Event;
const EventType = events.EventType;
const Key = events.Key;

pub const FeyInitializationError = error{
    FailedToInitializeSDL,
    FailedToCreateSDLWindow,
    FailedToGetNativeWindowHandle,
    FailedToInitializeBGFX,
};

pub const BackendType = enum {
    RAYLIB,
    SDL_BGFX,
};

const Backend = struct {
    backendType: BackendType,
    title: [:0]const u8,
    width: u32,
    height: u32,

    fn create(backendType: BackendType, title: [:0]const u8, width: u32, height: u32) !Backend {
        var backend = Backend{ .backendType = backendType, .title = title, .width = width, .height = height };
        try backend.init();
        return backend;
    }

    fn init(self: *Backend) !void {
        switch (self.backendType) {
            BackendType.RAYLIB => try self.init_raylib(),
            BackendType.SDL_BGFX => try self.init_sdl_bgfx(),
        }
    }

    fn pollEvent(self: *Backend, event: *Event) bool {
        switch (self.backendType) {
            BackendType.RAYLIB => return self.poll_event_raylib(event),
            BackendType.SDL_BGFX => return self.poll_event_sdl(event),
        }
    }

    fn init_raylib(self: *Backend) !void {
        _ = self;
        unreachable;
    }

    fn init_sdl_bgfx(self: *Backend) !void {
        if (!sdl.SDL_Init(0))
            return FeyInitializationError.FailedToInitializeSDL;

        const window = sdl.SDL_CreateWindow(
            self.title,
            @as(c_int, @intCast(self.width)),
            @as(c_int, @intCast(self.height)),
            sdl.SDL_WINDOW_RESIZABLE | sdl.SDL_WINDOW_HIGH_PIXEL_DENSITY,
        ) orelse {
            return FeyInitializationError.FailedToCreateSDLWindow;
        };

        _ = bgfx.bgfx_render_frame(0);

        var bgfx_init: bgfx.bgfx_init_t = .{};
        bgfx_init.type = bgfx.BGFX_RENDERER_TYPE_COUNT;

        const windowProperties = sdl.SDL_GetWindowProperties(window);

        // TODO: Add cross-platform logic of getting window handle:
        // https://wiki.libsdl.org/SDL3/SDL_GetWindowProperties
        const nativeWindowHandle =
            sdl.SDL_GetPointerProperty(windowProperties, sdl.SDL_PROP_WINDOW_COCOA_WINDOW_POINTER, null) orelse {
                return FeyInitializationError.FailedToGetNativeWindowHandle;
            };

        bgfx_init.platformData.nwh = nativeWindowHandle;
        bgfx_init.resolution.width = self.width;
        bgfx_init.resolution.height = self.height;
        bgfx_init.resolution.reset = bgfx.BGFX_RESET_VSYNC;

        bgfx_init.platformData.ndt = null;
        bgfx_init.platformData.context = null;
        bgfx_init.platformData.backBuffer = null;
        bgfx_init.platformData.backBufferDS = null;

        // On Apple's macOS, you must set the NSHighResolutionCapable Info.plist property to YES,
        // otherwise you will not receive a High-DPI OpenGL canvas.
        if (!bgfx.bgfx_init(&bgfx_init)) {
            return FeyInitializationError.FailedToInitializeBGFX;
        }

        bgfx.bgfx_set_view_rect(0, 0, 0, @intCast(self.width), @intCast(self.height)); // ! important !

        bgfx.bgfx_set_view_clear(0, bgfx.BGFX_CLEAR_COLOR | bgfx.BGFX_CLEAR_DEPTH, 0x303030ff, 0, 0);
        _ = bgfx.bgfx_frame(false);
    }

    fn poll_event_raylib(self: *Backend, event: *Event) bool {
        _ = self;
        _ = event;
        unreachable;
    }

    fn poll_event_sdl(self: *Backend, event: *Event) bool {
        _ = self;
        var sdlEvent: sdl.SDL_Event = undefined;
        const result = sdl.SDL_PollEvent(&sdlEvent);
        event.* = Event.fromSDL(sdlEvent);
        return result;
    }
};

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
