const std = @import("std");

const sdl = @import("sdl.zig").sdl;
const bgfx = @import("bgfx.zig").bgfx;

const events = @import("events.zig");
const renderer = @import("renderer.zig");

const Event = events.Event;
const Renderer = renderer.Renderer;

pub const FeyInitializationError = error{
    FailedToInitializeSDL,
    FailedToCreateSDLWindow,
    FailedToGetNativeWindowHandle,
    FailedToInitializeBGFX,
};

pub const Backend = struct {
    allocator: std.mem.Allocator,
    title: [:0]const u8,
    width: u32,
    height: u32,
    window: ?*sdl.struct_SDL_Window = null,
    renderer: Renderer,

    pub fn init(allocator: std.mem.Allocator, title: [:0]const u8, width: u32, height: u32) !Backend {
        var backend = Backend{
            .allocator = allocator,
            .title = title,
            .width = width,
            .height = height,
            .renderer = undefined,
        };
        try backend.initInternal();
        // have to be created after BGFX initialization
        backend.renderer = try Renderer.init(allocator, width, height);
        return backend;
    }

    pub fn deinit(self: *Backend) void {
        self.renderer.deinit();
        bgfx.bgfx_shutdown();
        sdl.SDL_DestroyWindow(self.window);
        sdl.SDL_Quit();
    }

    pub fn pollEvent(self: *Backend, event: *Event) bool {
        _ = self;

        var sdlEvent: sdl.SDL_Event = undefined;
        const result = sdl.SDL_PollEvent(&sdlEvent);
        event.* = Event.fromSDL(sdlEvent);
        return result;
    }

    fn initInternal(self: *Backend) !void {
        if (!sdl.SDL_Init(0))
            return FeyInitializationError.FailedToInitializeSDL;

        self.window = sdl.SDL_CreateWindow(
            self.title,
            @as(c_int, @intCast(self.width)),
            @as(c_int, @intCast(self.height)),
            sdl.SDL_WINDOW_RESIZABLE | sdl.SDL_WINDOW_HIGH_PIXEL_DENSITY,
        ) orelse {
            return FeyInitializationError.FailedToCreateSDLWindow;
        };

        _ = bgfx.bgfx_render_frame(0);

        var bgfx_init: bgfx.bgfx_init_t = .{};
        bgfx.bgfx_init_ctor(&bgfx_init);

        bgfx_init.type = bgfx.BGFX_RENDERER_TYPE_COUNT;

        const windowProperties = sdl.SDL_GetWindowProperties(self.window);

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

        self.renderer.fill(0xff0000ff, .{});
        _ = bgfx.bgfx_frame(false);
    }
};
