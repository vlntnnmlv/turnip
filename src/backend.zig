const sdl = @import("sdl.zig").sdl;
const bgfx = @import("bgfx.zig").bgfx;

const events = @import("events.zig");

const Event = events.Event;

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

pub const Backend = struct {
    init_f: *const fn ([:0]const u8, u32, u32) anyerror!void,
    poll_event_f: *const fn (*Event) bool,

    backendType: BackendType,
    title: [:0]const u8,
    width: u32,
    height: u32,

    pub fn create(backendType: BackendType, title: [:0]const u8, width: u32, height: u32) !Backend {
        var backend = Backend{
            .init_f = switch (backendType) {
                BackendType.RAYLIB => &Backend.init_raylib,
                BackendType.SDL_BGFX => &Backend.init_sdl_bgfx,
            },
            .poll_event_f = switch (backendType) {
                BackendType.RAYLIB => &Backend.poll_event_raylib,
                BackendType.SDL_BGFX => &Backend.poll_event_sdl,
            },
            .backendType = backendType,
            .title = title,
            .width = width,
            .height = height,
        };
        try backend.init();
        return backend;
    }

    fn init(self: *Backend) !void {
        try self.init_f(self.title, self.width, self.height);
    }

    pub fn pollEvent(self: *Backend, event: *Event) bool {
        return self.poll_event_f(event);
    }

    fn init_raylib(title: [:0]const u8, width: u32, height: u32) !void {
        _ = title;
        _ = width;
        _ = height;
        unreachable;
    }

    fn init_sdl_bgfx(title: [:0]const u8, width: u32, height: u32) !void {
        if (!sdl.SDL_Init(0))
            return FeyInitializationError.FailedToInitializeSDL;

        const window = sdl.SDL_CreateWindow(
            title,
            @as(c_int, @intCast(width)),
            @as(c_int, @intCast(height)),
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
        bgfx_init.resolution.width = width;
        bgfx_init.resolution.height = height;
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

        bgfx.bgfx_set_view_rect(0, 0, 0, @intCast(width), @intCast(height)); // ! important !

        bgfx.bgfx_set_view_clear(0, bgfx.BGFX_CLEAR_COLOR | bgfx.BGFX_CLEAR_DEPTH, 0x303030ff, 0, 0);
        _ = bgfx.bgfx_frame(false);
    }

    fn poll_event_raylib(event: *Event) bool {
        _ = event;
        unreachable;
    }

    fn poll_event_sdl(event: *Event) bool {
        var sdlEvent: sdl.SDL_Event = undefined;
        const result = sdl.SDL_PollEvent(&sdlEvent);
        event.* = Event.fromSDL(sdlEvent);
        return result;
    }
};
