const std = @import("std");

const bgfx = @import("bgfx.zig").bgfx;
const zlm = @import("zlm").as(f32);

const geometry = @import("geometry.zig");
const backend = @import("backend.zig");
const assetLoader = @import("assetLoader.zig");

const AssetLoader = assetLoader.AssetLoader;

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

pub const Vertex2D = struct {
    x: f32,
    y: f32,
    z: f32,
    u: f32,
    v: f32,

    pub fn fromRectangle(allocator: std.mem.Allocator, rectangle: geometry.Rectangle) [4]Vertex2D {
        // const mesh = try allocator.create([4]Vertex2D);
        _ = allocator;
        const xMin: f32 = rectangle.x;
        const xMax: f32 = rectangle.x + rectangle.width;

        const yMin: f32 = rectangle.y;
        const yMax: f32 = rectangle.y + rectangle.height;
        return .{
            .{ .x = xMin, .y = yMin, .z = 0, .u = 0, .v = 0 },
            .{ .x = xMax, .y = yMin, .z = 0, .u = 1, .v = 0 },
            .{ .x = xMax, .y = yMax, .z = 0, .u = 1, .v = 1 },
            .{ .x = xMin, .y = yMax, .z = 0, .u = 0, .v = 1 },
        };

        // return mesh;
    }
};

// Renders:
// - Primitives (Line, Square, Rectangle, Circle, Oval, Triangle etc)
// - Textures,
// - Models

fn setupViewProjection(width: u32, height: u32) struct { view: [16]f32, proj: [16]f32 } {
    // Create orthographic projection for 2D

    const proj = zlm.Mat4.createOrthogonal(
        0.0, // left
        @as(f32, @floatFromInt(width)), // right
        @as(f32, @floatFromInt(height)), // bottom (flipped for BGFX)
        0.0, // top
        -1.0, // near
        1.0, // far
    );

    // Identity view matrix
    const view = zlm.Mat4.identity;

    return .{
        .view = @bitCast(view),
        .proj = @bitCast(proj),
    };
}

pub const Renderer = struct {
    allocator: std.mem.Allocator,
    width: u32,
    height: u32,

    var vertex_2d_layout: bgfx.bgfx_vertex_layout_t = undefined;
    var uv_texture_shader_program: bgfx.bgfx_program_handle_t = undefined;
    var view_matrix: [16]f32 = undefined;
    var projecion_matrix: [16]f32 = undefined;

    pub fn init(allocator: std.mem.Allocator, width: u32, height: u32) !Renderer {
        _ = bgfx.bgfx_vertex_layout_begin(&vertex_2d_layout, bgfx.BGFX_RENDERER_TYPE_COUNT);
        _ = bgfx.bgfx_vertex_layout_add(&vertex_2d_layout, bgfx.BGFX_ATTRIB_POSITION, 3, bgfx.BGFX_ATTRIB_TYPE_FLOAT, false, false);
        _ = bgfx.bgfx_vertex_layout_add(&vertex_2d_layout, bgfx.BGFX_ATTRIB_COLOR0, 3, bgfx.BGFX_ATTRIB_TYPE_UINT8, false, false);
        bgfx.bgfx_vertex_layout_end(&vertex_2d_layout);

        const vs = try AssetLoader.loadShader("quad_vs");
        const fs = try AssetLoader.loadShader("quad_fs");
        uv_texture_shader_program = bgfx.bgfx_create_program(vs, fs, true);

        const mxs = setupViewProjection(width, height);
        view_matrix = mxs.view;
        projecion_matrix = mxs.proj;

        return Renderer{ .allocator = allocator, .width = width, .height = height };
    }

    pub fn deinit(self: *Renderer) void {
        _ = self;
    }

    pub fn beginRender(self: *Renderer) void {
        bgfx.bgfx_set_view_transform(0, &view_matrix, &projecion_matrix);
        self.setViewRect(geometry.Rectangle{
            .x = 0,
            .y = 0,
            .width = @floatFromInt(self.width),
            .height = @floatFromInt(self.height),
        });
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

    // var view_matrix = undefined;
    // var proj_matrix = undefined;

    pub fn renderTexture(self: *Renderer, textureHandle: bgfx.bgfx_texture_handle_t, rectangle: geometry.Rectangle) void {
        const quad_mesh = Vertex2D.fromRectangle(self.allocator, rectangle);

        const vertex_buffer =
            bgfx.bgfx_create_vertex_buffer(
                bgfx.bgfx_copy(&quad_mesh, 4 * @sizeOf(Vertex2D)),
                &vertex_2d_layout,
                0,
            );

        const triangles_buffer: bgfx.bgfx_index_buffer_handle_t = bgfx.bgfx_create_index_buffer(
            bgfx.bgfx_copy(&[6]u16{ 0, 2, 1, 0, 3, 2 }, 6 * @sizeOf(u16)),
            0,
        );

        const texture_sampler_uniform: bgfx.bgfx_uniform_handle_t =
            bgfx.bgfx_create_uniform("s_texColor", bgfx.BGFX_UNIFORM_TYPE_SAMPLER, 1);

        bgfx.bgfx_set_vertex_buffer(0, vertex_buffer, 0, 4);
        bgfx.bgfx_set_index_buffer(triangles_buffer, 0, 6);

        bgfx.bgfx_set_texture(0, texture_sampler_uniform, textureHandle, 4294967295);
        bgfx.bgfx_set_state(bgfx.BGFX_STATE_DEFAULT | bgfx.BGFX_STATE_BLEND_ALPHA, 0);
        bgfx.bgfx_submit(0, uv_texture_shader_program, 0, 255);
    }
};
