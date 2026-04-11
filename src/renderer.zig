const std = @import("std");

const bgfx = @import("bgfx.zig").bgfx;

const zlm = @import("zlm").as(f32);

const geometry = @import("geometry.zig");
const components = @import("components.zig");
const backend = @import("backend.zig");
const assetLoader = @import("asset_loader.zig");

const Camera = components.Camera;
const Mesh = components.Mesh;
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
fn setupOrthogonalViewProjection(width: f32, height: f32, near: f32, far: f32, position: zlm.Vec3) ViewProjection {
    var proj = zlm.Mat4.createOrthogonal(0.0, width, height, 0.0, near, far);

    const caps = bgfx.bgfx_get_caps();
    if (!caps.*.homogeneousDepth) {
        proj.fields[2][2] *= 0.5;
        proj.fields[2][3] *= 0.5;
        proj.fields[3][2] = proj.fields[3][2] * 0.5 + 0.5;
    }

    const view = zlm.Mat4.createTranslation(position.scale(-1.0));

    return ViewProjection{
        .view = @bitCast(view),
        .proj = @bitCast(proj),
    };
}

pub fn setupPerspectiveViewProjection(
    width: f32,
    height: f32,
    fov_degrees: f32,
    near: f32,
    far: f32,
    position: zlm.Vec3,
    // rotation: zlm.Vec3,
    target: zlm.Vec3,
    up: zlm.Vec3,
) ViewProjection {
    const aspect_ratio = width / height;
    const fov_radians = zlm.toRadians(fov_degrees);

    const proj = zlm.Mat4.createPerspective(fov_radians, aspect_ratio, near, far);

    // const translation = zlm.Mat4.createTranslation(position.scale(-1.0));
    // const rot = zlm.Mat4.fromQuat(rotation.conjugate());
    // const view = zlm.Mat4.createLookAt(position, target, up);
    // const view = rot.mul(translation);
    const view = zlm.Mat4.createLookAt(position, target, up);

    return ViewProjection{
        .view = @bitCast(view),
        .proj = @bitCast(proj),
    };
}

pub const Renderer = struct {
    allocator: std.mem.Allocator,
    width: u32,
    height: u32,
    vertex_2d_layout: bgfx.bgfx_vertex_layout_t,
    shader_program: bgfx.bgfx_program_handle_t,
    texture_sampler_uniform: bgfx.bgfx_uniform_handle_t,

    view_matrix: [16]f32 = undefined,
    projection_matrix: [16]f32 = undefined,

    const Self = @This();

    pub fn init(allocator: std.mem.Allocator, width: u32, height: u32) !Renderer {
        var layout: bgfx.bgfx_vertex_layout_t = undefined;
        _ = bgfx.bgfx_vertex_layout_begin(&layout, bgfx.BGFX_RENDERER_TYPE_COUNT);
        _ = bgfx.bgfx_vertex_layout_add(&layout, bgfx.BGFX_ATTRIB_POSITION, 3, bgfx.BGFX_ATTRIB_TYPE_FLOAT, false, false);
        _ = bgfx.bgfx_vertex_layout_add(&layout, bgfx.BGFX_ATTRIB_TEXCOORD0, 2, bgfx.BGFX_ATTRIB_TYPE_FLOAT, false, false);
        bgfx.bgfx_vertex_layout_end(&layout);

        const vs = try AssetLoader.loadShader(allocator, "quad_vs");
        const fs = try AssetLoader.loadShader(allocator, "quad_fs");

        return Renderer{
            .allocator = allocator,
            .width = width,
            .height = height,
            .vertex_2d_layout = layout,
            .shader_program = bgfx.bgfx_create_program(vs, fs, true),
            .texture_sampler_uniform = bgfx.bgfx_create_uniform("s_texColor", bgfx.BGFX_UNIFORM_TYPE_SAMPLER, 1),
        };
    }

    fn destroyHandle(handle: anytype) void {
        const handle_type = @TypeOf(handle);
        if (!bgfx.BGFX_HANDLE_IS_VALID(handle)) return;
        switch (handle_type) {
            bgfx.bgfx_uniform_handle_s => bgfx.bgfx_destroy_uniform(handle),
            bgfx.bgfx_index_buffer_handle_s => bgfx.bgfx_destroy_index_buffer(handle),
            bgfx.bgfx_vertex_buffer_handle_s => bgfx.bgfx_destroy_vertex_buffer(handle),
            bgfx.bgfx_program_handle_s => bgfx.bgfx_destroy_program(handle),
            else => {},
        }
    }

    pub fn deinit(self: *Renderer) void {
        destroyHandle(self.texture_sampler_uniform);
        destroyHandle(self.shader_program);
    }

    pub fn setCamera(self: *Renderer, camera: Camera) void {
        const near = camera.getNear();
        const far = camera.getFar();

        const mxs = switch (camera.view_type) {
            Camera.ViewType.ORTHOGONAL => setupOrthogonalViewProjection(
                camera.options.view_rectangle.width,
                camera.options.view_rectangle.height,
                near,
                far,
                camera.position,
            ),
            Camera.ViewType.PERSPECTIVE => setupPerspectiveViewProjection(
                camera.options.view_rectangle.width,
                camera.options.view_rectangle.height,
                camera.options.fov,
                near,
                far,
                camera.position,
                camera.getTarget(),
                camera.options.up,
            ),
        };

        self.view_matrix = mxs.view;
        self.projection_matrix = mxs.proj;

        self.setViewRect(camera.options.view_rectangle);
        bgfx.bgfx_set_view_transform(0, &self.view_matrix, &self.projection_matrix);
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
            depth: f32 = 1.0,
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

    pub fn renderMesh(self: *Renderer, textureHandle: bgfx.bgfx_texture_handle_t, mesh: Mesh) !void {
        const num_vertices: u32 = @intCast(mesh.vertices.len);
        const num_indices: u32 = @intCast(mesh.indices.len);

        var tvb: bgfx.bgfx_transient_vertex_buffer_t = undefined;
        var tib: bgfx.bgfx_transient_index_buffer_t = undefined;

        // Check if transient buffers have enough space
        if (bgfx.bgfx_get_avail_transient_vertex_buffer(num_vertices, &self.vertex_2d_layout) != num_vertices) return;
        if (bgfx.bgfx_get_avail_transient_index_buffer(num_indices, true) != num_indices) return;

        bgfx.bgfx_alloc_transient_vertex_buffer(&tvb, num_vertices, &self.vertex_2d_layout);
        bgfx.bgfx_alloc_transient_index_buffer(&tib, num_indices, true);

        const verts: [*]Vertex2D = @ptrCast(@alignCast(tvb.data));
        for (0..mesh.vertices.len) |i| {
            verts[i] = Vertex2D{
                .x = mesh.vertices[i].x,
                .y = mesh.vertices[i].y,
                .z = mesh.vertices[i].z,
                .u = mesh.uvs[i].x,
                .v = mesh.uvs[i].y,
            };
        }

        const idx: [*]u32 = @ptrCast(@alignCast(tib.data));
        @memcpy(idx[0..mesh.indices.len], mesh.indices);

        bgfx.bgfx_set_transient_vertex_buffer(0, &tvb, 0, num_vertices);
        bgfx.bgfx_set_transient_index_buffer(&tib, 0, num_indices);

        bgfx.bgfx_set_texture(0, self.texture_sampler_uniform, textureHandle, 4294967295);

        const state: u64 = bgfx.BGFX_STATE_WRITE_RGB |
            bgfx.BGFX_STATE_WRITE_A |
            bgfx.BGFX_STATE_WRITE_Z |
            bgfx.BGFX_STATE_BLEND_ALPHA |
            bgfx.BGFX_STATE_CULL_CW |
            bgfx.BGFX_STATE_MSAA;

        bgfx.bgfx_set_state(state, 0);
        bgfx.bgfx_submit(0, self.shader_program, 0, bgfx.BGFX_DISCARD_ALL);
    }

    pub fn renderTexture(self: *Renderer, textureHandle: bgfx.bgfx_texture_handle_t, rectangle: geometry.Rectangle) !void {
        _ = self.allocator; // no longer needed for per-call allocations

        const num_vertices: u32 = 4;
        const num_indices: u32 = 6;

        var tvb: bgfx.bgfx_transient_vertex_buffer_t = undefined;
        var tib: bgfx.bgfx_transient_index_buffer_t = undefined;

        if (bgfx.bgfx_get_avail_transient_vertex_buffer(num_vertices, &self.vertex_2d_layout) != num_vertices) return;
        if (bgfx.bgfx_get_avail_transient_index_buffer(num_indices, false) != num_indices) return;

        bgfx.bgfx_alloc_transient_vertex_buffer(&tvb, num_vertices, &self.vertex_2d_layout);
        bgfx.bgfx_alloc_transient_index_buffer(&tib, num_indices, false);

        const xMin: f32 = rectangle.x;
        const xMax: f32 = rectangle.x + rectangle.width;
        const yMin: f32 = rectangle.y;
        const yMax: f32 = rectangle.y + rectangle.height;

        const verts: [*]Vertex2D = @ptrCast(@alignCast(tvb.data));
        verts[0] = Vertex2D{ .x = xMin, .y = yMin, .z = 0, .u = 0, .v = 0 };
        verts[1] = Vertex2D{ .x = xMax, .y = yMin, .z = 0, .u = 1, .v = 0 };
        verts[2] = Vertex2D{ .x = xMax, .y = yMax, .z = 0, .u = 1, .v = 1 };
        verts[3] = Vertex2D{ .x = xMin, .y = yMax, .z = 0, .u = 0, .v = 1 };

        const idx: [*]u16 = @ptrCast(@alignCast(tib.data));
        const quad_indices = [6]u16{ 0, 2, 1, 0, 3, 2 };
        @memcpy(idx[0..6], &quad_indices);

        bgfx.bgfx_set_transient_vertex_buffer(0, &tvb, 0, num_vertices);
        bgfx.bgfx_set_transient_index_buffer(&tib, 0, num_indices);

        bgfx.bgfx_set_texture(0, self.texture_sampler_uniform, textureHandle, 4294967295);

        const state: u64 = bgfx.BGFX_STATE_WRITE_RGB |
            bgfx.BGFX_STATE_WRITE_A |
            bgfx.BGFX_STATE_WRITE_Z |
            bgfx.BGFX_STATE_BLEND_ALPHA |
            bgfx.BGFX_STATE_CULL_CW |
            bgfx.BGFX_STATE_MSAA;

        bgfx.bgfx_set_state(state, 0);
        bgfx.bgfx_submit(0, self.shader_program, 0, bgfx.BGFX_DISCARD_ALL);
    }
};
