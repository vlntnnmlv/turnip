const std = @import("std");

const bgfx = @import("bgfx.zig").bgfx;

const zigimg = @import("zigimg");

pub const AssetLoader = struct {
    pub fn loadShader(name: []const u8) !bgfx.bgfx_shader_handle_t {
        // const renderType: []const u8 = switch (bgfx.bgfx_get_renderer_type()) {
        //     bgfx.BGFX_RENDERER_TYPE_NOOP => "dx9/",
        //     bgfx.BGFX_RENDERER_TYPE_DIRECT3D11 => "dx11/",
        //     bgfx.BGFX_RENDERER_TYPE_DIRECT3D12 => "dx11/",
        //     bgfx.BGFX_RENDERER_TYPE_GNM => "pssl/",
        //     bgfx.BGFX_RENDERER_TYPE_METAL => "metal/",
        //     bgfx.BGFX_RENDERER_TYPE_OPENGL => "glsl/",
        //     bgfx.BGFX_RENDERER_TYPE_OPENGLES => "essl/",
        //     bgfx.BGFX_RENDERER_TYPE_VULKAN => "spirv/",
        //     bgfx.BGFX_RENDERER_TYPE_NVN => unreachable,
        //     bgfx.BGFX_RENDERER_TYPE_COUNT => unreachable,
        //     else => unreachable,
        // };

        const allocator = std.heap.c_allocator;

        var full_path_builder: std.ArrayList(u8) = .empty;
        defer full_path_builder.deinit(std.heap.c_allocator);

        try full_path_builder.appendSlice(std.heap.c_allocator, "resources/shaders/");
        // try full_path_builder.appendSlice(std.heap.c_allocator, renderType);
        try full_path_builder.appendSlice(std.heap.c_allocator, name);
        try full_path_builder.appendSlice(std.heap.c_allocator, ".bin");

        const shader_file = try std.fs.cwd().openFile(full_path_builder.items, .{});
        defer shader_file.close();

        const shader_stat = try shader_file.stat();
        const shader_size: u32 = @intCast(shader_stat.size);
        const shader_content = try shader_file.readToEndAlloc(allocator, shader_size + 1);
        defer allocator.free(shader_content); // Free the allocated memory

        const mem = bgfx.bgfx_alloc(shader_size + 1);
        const mem_mut: *bgfx.bgfx_memory_t = @ptrCast(@constCast(mem));
        @memmove(mem_mut.data[0..shader_size], shader_content);
        mem_mut.data[shader_size] = 0;

        const shader_handle = bgfx.bgfx_create_shader(mem);
        return bgfx.bgfx_shader_handle_t{ .idx = shader_handle.idx };
    }

    pub fn loadTexture(name: []const u8) !bgfx.bgfx_texture_handle_t {
        const allocator = std.heap.c_allocator;
        var full_path_builder: std.ArrayList(u8) = .empty;
        defer full_path_builder.deinit(allocator);

        try full_path_builder.appendSlice(allocator, "resources/textures/");
        try full_path_builder.appendSlice(allocator, name);
        try full_path_builder.appendSlice(allocator, ".png");
        // try full_path_builder.append(allocator, 0);

        var read_buffer: [16 * 1024]u8 = undefined;
        var image = try zigimg.Image.fromFilePath(allocator, full_path_builder.items, &read_buffer);
        defer image.deinit(allocator);

        const rgba_image = switch (image.pixels) {
            .rgba32 => |pixels| pixels,
            else => unreachable, //try image.toRGBA32(allocator),
        };
        defer if (image.pixels != .rgba32) allocator.free(rgba_image);

        const width = image.width;
        const height = image.height;

        std.debug.print("[DEBUG] Image loaded: {}x{}\n", .{ width, height });

        // TODO: Use this function from AssetManager, and free all the memory from there.
        const textureHandle = bgfx.bgfx_create_texture_2d(
            @intCast(width),
            @intCast(height),
            false,
            1,
            bgfx.BGFX_TEXTURE_FORMAT_RGBA8,
            bgfx.BGFX_TEXTURE_NONE | bgfx.BGFX_SAMPLER_U_CLAMP | bgfx.BGFX_SAMPLER_V_CLAMP | bgfx.BGFX_SAMPLER_POINT,
            bgfx.bgfx_copy(rgba_image.ptr, @as(u32, @intCast(width * height * 4))),
        );

        return textureHandle;
    }
};
