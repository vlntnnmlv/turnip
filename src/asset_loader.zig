const std = @import("std");
const bgfx = @import("bgfx.zig").bgfx;
const zigimg = @import("zigimg");
const fey_asset = @import("fey_asset");

pub const fbx = fey_asset.fbx;

pub const AssetLoader = struct {
    pub fn loadShader(allocator: std.mem.Allocator, name: []const u8) !bgfx.bgfx_shader_handle_t {
        var full_path_builder: std.ArrayList(u8) = .empty;
        defer full_path_builder.deinit(allocator);

        try full_path_builder.appendSlice(allocator, "resources/shaders/");
        try full_path_builder.appendSlice(allocator, name);
        try full_path_builder.appendSlice(allocator, ".bin");

        const shader_file = try std.fs.cwd().openFile(full_path_builder.items, .{});
        defer shader_file.close();

        const shader_stat = try shader_file.stat();
        const shader_size: u32 = @intCast(shader_stat.size);
        const shader_content = try shader_file.readToEndAlloc(allocator, shader_size + 1);
        defer allocator.free(shader_content);

        const mem = bgfx.bgfx_alloc(shader_size + 1);
        const mem_mut: *bgfx.bgfx_memory_t = @ptrCast(@constCast(mem));
        @memmove(mem_mut.data[0..shader_size], shader_content);
        mem_mut.data[shader_size] = 0;

        const shader_handle = bgfx.bgfx_create_shader(mem);
        return bgfx.bgfx_shader_handle_t{ .idx = shader_handle.idx };
    }

    pub fn loadTexture(allocator: std.mem.Allocator, name: []const u8) !bgfx.bgfx_texture_handle_t {
        var full_path_builder: std.ArrayList(u8) = .empty;
        defer full_path_builder.deinit(allocator);

        try full_path_builder.appendSlice(allocator, "resources/textures/");
        try full_path_builder.appendSlice(allocator, name);
        try full_path_builder.appendSlice(allocator, ".png");

        var read_buffer: [16 * 1024]u8 = undefined;
        var image = try zigimg.Image.fromFilePath(allocator, full_path_builder.items, &read_buffer);
        defer image.deinit(allocator);

        const rgba_image = switch (image.pixels) {
            .rgba32 => |pixels| pixels,
            else => unreachable,
        };
        defer if (image.pixels != .rgba32) allocator.free(rgba_image);

        const width = image.width;
        const height = image.height;

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

    pub fn loadFBX(allocator: std.mem.Allocator, comptime name: []const u8) !fbx.FBXFile {
        const path = "resources/models/" ++ name ++ ".fbx";
        return try fbx.FBXFile.init(allocator, path);
    }
};
