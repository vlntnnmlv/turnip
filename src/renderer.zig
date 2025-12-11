const std = @import("std");

const bgfx = @import("bgfx.zig").bgfx;
const zlm = @import("zlm").as(f32);

const geometry = @import("geometry.zig");
const ecs = @import("ecs.zig");
const backend = @import("backend.zig");
const assetLoader = @import("asset_loader.zig");

const Camera = ecs.components.Camera;
const AssetLoader = assetLoader.AssetLoader;

pub const Vertex2D = struct {
    x: f32,
    y: f32,
    z: f32,
    u: f32,
    v: f32,

    pub fn fromRectangle(allocator: std.mem.Allocator, rectangle: geometry.Rectangle) ![]Vertex2D {
        const xMin: f32 = rectangle.x;
        const xMax: f32 = rectangle.x + rectangle.width;

        const yMin: f32 = rectangle.y;
        const yMax: f32 = rectangle.y + rectangle.height;

        var mesh = try allocator.alloc(Vertex2D, 4);
        mesh[0] = Vertex2D{ .x = xMin, .y = yMin, .z = 0, .u = 0, .v = 0 };
        mesh[1] = Vertex2D{ .x = xMax, .y = yMin, .z = 0, .u = 1, .v = 0 };
        mesh[2] = Vertex2D{ .x = xMax, .y = yMax, .z = 0, .u = 1, .v = 1 };
        mesh[3] = Vertex2D{ .x = xMin, .y = yMax, .z = 0, .u = 0, .v = 1 };
        return mesh;
    }
};

const ViewProjection = struct { view: [16]f32, proj: [16]f32 };
fn setupViewProjection(width: f32, height: f32) ViewProjection {
    const proj = zlm.Mat4.createOrthogonal(0.0, width, height, 0.0, -1.0, 1.0);
    const view = zlm.Mat4.identity;

    return ViewProjection{
        .view = @bitCast(view),
        .proj = @bitCast(proj),
    };
}

pub fn setupPerspectiveViewProjection(width: f32, height: f32, fov_degrees: f32, near: f32, far: f32) ViewProjection {
    const aspect_ratio = width / height;
    const fov_radians = zlm.toRadians(fov_degrees);

    const proj = zlm.Mat4.createPerspective(fov_radians, aspect_ratio, near, far);
    const view = zlm.Mat4.identity;

    return ViewProjection{
        .view = @bitCast(view),
        .proj = @bitCast(proj),
    };
}

// TODO: Fix leaks
pub const Renderer = struct {
    allocator: std.mem.Allocator,
    width: u32,
    height: u32,

    var vertex_2d_layout: bgfx.bgfx_vertex_layout_t = undefined;
    var uv_texture_shader_program: bgfx.bgfx_program_handle_t = undefined;

    var vertex_buffer: bgfx.bgfx_vertex_buffer_handle_t = undefined;
    var triangles_buffer: bgfx.bgfx_index_buffer_handle_t = undefined;
    var texture_sampler_uniform: bgfx.bgfx_uniform_handle_t = undefined;

    var view_matrix: [16]f32 = undefined;
    var projecion_matrix: [16]f32 = undefined;

    pub fn init(allocator: std.mem.Allocator, width: u32, height: u32) !Renderer {
        _ = bgfx.bgfx_vertex_layout_begin(&vertex_2d_layout, bgfx.BGFX_RENDERER_TYPE_COUNT);
        _ = bgfx.bgfx_vertex_layout_add(&vertex_2d_layout, bgfx.BGFX_ATTRIB_POSITION, 3, bgfx.BGFX_ATTRIB_TYPE_FLOAT, false, false);
        _ = bgfx.bgfx_vertex_layout_add(&vertex_2d_layout, bgfx.BGFX_ATTRIB_TEXCOORD0, 2, bgfx.BGFX_ATTRIB_TYPE_FLOAT, false, false);
        bgfx.bgfx_vertex_layout_end(&vertex_2d_layout);

        const vs = try AssetLoader.loadShader("quad_vs");
        const fs = try AssetLoader.loadShader("quad_fs");
        uv_texture_shader_program = bgfx.bgfx_create_program(vs, fs, true);

        triangles_buffer =
            bgfx.bgfx_create_index_buffer(
                bgfx.bgfx_copy(&[6]u16{ 0, 2, 1, 0, 3, 2 }, 6 * @sizeOf(u16)),
                0,
            );

        texture_sampler_uniform =
            bgfx.bgfx_create_uniform("s_texColor", bgfx.BGFX_UNIFORM_TYPE_SAMPLER, 1);

        return Renderer{ .allocator = allocator, .width = width, .height = height };
    }

    fn destroyHandle(handle: anytype) void {
        const handle_type = @TypeOf(handle);
        std.debug.print("[DEBUG] handle type: {}\n", .{handle_type});
        if (!bgfx.BGFX_HANDLE_IS_VALID(handle)) {
            std.debug.print("[DEBUG] handle is invalid\n", .{});
        }
        switch (handle_type) {
            bgfx.bgfx_uniform_handle_s => bgfx.bgfx_destroy_uniform(handle),
            bgfx.bgfx_index_buffer_handle_s => bgfx.bgfx_destroy_index_buffer(handle),
            bgfx.bgfx_vertex_buffer_handle_s => bgfx.bgfx_destroy_vertex_buffer(handle),
            bgfx.bgfx_program_handle_s => bgfx.bgfx_destroy_program(handle),
            else => {},
        }
        std.debug.print("[DEBUG] handle destroyed\n", .{});
    }

    pub fn deinit(self: *Renderer) void {
        _ = self;

        destroyHandle(texture_sampler_uniform);
        destroyHandle(triangles_buffer);
        // destroyHandle(vertex_buffer);
        destroyHandle(uv_texture_shader_program);
    }

    pub fn setCamera(self: *Renderer, camera: Camera) void {
        const mxs = switch (camera.view_type) {
            Camera.ViewType.ORTHOGONAL => setupViewProjection(
                camera.options.view_rectangle.width,
                camera.options.view_rectangle.height,
            ),
            Camera.ViewType.PERSPECTIVE => setupPerspectiveViewProjection(
                camera.options.view_rectangle.width,
                camera.options.view_rectangle.height,
                camera.options.fov,
                camera.options.near,
                camera.options.far,
            ),
        };

        view_matrix = mxs.view;
        projecion_matrix = mxs.proj;

        self.setViewRect(camera.options.view_rectangle);
        bgfx.bgfx_set_view_transform(0, &view_matrix, &projecion_matrix);
    }

    pub fn beginRender(self: *Renderer) void {
        _ = self;

        bgfx.bgfx_touch(0);
    }

    pub fn endRender(self: *Renderer) void {
        _ = self;

        _ = bgfx.bgfx_frame(false);
    }

    pub fn fill(
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

    pub fn renderTexture(self: *Renderer, textureHandle: bgfx.bgfx_texture_handle_t, rectangle: geometry.Rectangle) !void {
        const quad_mesh = try Vertex2D.fromRectangle(self.allocator, rectangle);
        defer self.allocator.free(quad_mesh);

        if (bgfx.BGFX_HANDLE_IS_VALID(vertex_buffer)) {
            bgfx.bgfx_destroy_vertex_buffer(vertex_buffer);
        }

        vertex_buffer =
            bgfx.bgfx_create_vertex_buffer(
                bgfx.bgfx_copy(quad_mesh.ptr, 4 * @sizeOf(Vertex2D)),
                &vertex_2d_layout,
                0,
            );

        bgfx.bgfx_set_vertex_buffer(0, vertex_buffer, 0, 4);
        bgfx.bgfx_set_index_buffer(triangles_buffer, 0, 6);

        bgfx.bgfx_set_texture(0, texture_sampler_uniform, textureHandle, 4294967295);

        // https://bkaradzic.github.io/bgfx/bgfx.html - bgfx states
        const state: u64 = bgfx.BGFX_STATE_WRITE_RGB |
            bgfx.BGFX_STATE_WRITE_A |
            bgfx.BGFX_STATE_WRITE_Z |
            // bgfx.BGFX_STATE_DEPTH_TEST_LESS | // TODO: This ruins 2d rendering for some reason...
            bgfx.BGFX_STATE_BLEND_ALPHA |
            bgfx.BGFX_STATE_CULL_CW |
            bgfx.BGFX_STATE_MSAA;

        bgfx.bgfx_set_state(state, 0);
        bgfx.bgfx_submit(0, uv_texture_shader_program, 0, bgfx.BGFX_DISCARD_ALL);
    }
};
