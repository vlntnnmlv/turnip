const std = @import("std");

const bgfx = @import("bgfx.zig").bgfx;
const geometry = @import("geometry.zig");
const backend = @import("backend.zig");

const BackendType = backend.BackendType;

const Texture = struct {};
const BeginRenderFT = *const fn () void;
const EndRenderFT = *const fn () void;

const RenderViewColorFT = *const fn (
    u32,
    struct {
        flags: u16 = bgfx.BGFX_CLEAR_COLOR | bgfx.BGFX_CLEAR_DEPTH,
        depth: f32 = 0.0,
        stencil: u8 = 0,
    },
) void;

// Renders:
// - Primitives (Line, Square, Rectangle, Circle, Oval, Triangle etc)
// - Textures,
// - Models

pub const Renderer = struct {
    pub fn create() Renderer {
        return Renderer{};
    }

    pub fn beginRender(self: *Renderer) void {
        _ = self;

        bgfx.bgfx_touch(0);
    }

    pub fn endRender(self: *Renderer) void {
        _ = self;

        _ = bgfx.bgfx_frame(false);
    }

    pub fn renderViewColor(
        self: *Renderer,
        color: u32,
        options: struct {
            flags: u16 = bgfx.BGFX_CLEAR_COLOR | bgfx.BGFX_CLEAR_DEPTH,
            depth: f32 = 0.0,
            stencil: u8 = 0,
        },
    ) void {
        _ = self;

        bgfx.bgfx_set_view_clear(0, options.flags, color, options.depth, options.stencil);
    }

    pub fn setViewRect(self: *Renderer, rectangle: geometry.Rectangle) void {
        _ = self;

        bgfx.bgfx_set_view_rect(
            0,
            @intFromFloat(rectangle.x),
            @intFromFloat(rectangle.y),
            @intFromFloat(rectangle.width),
            @intFromFloat(rectangle.height),
        );
    }
};
