// 1. build bgfx and sdl
// 2. build fey
// 3. build examples

const std = @import("std");

fn buildShader(b: *std.Build, shader_path: []const u8, shader_type: u8) !*std.Build.Step.Run {
    const shaderc = "./vendor/bgfx.cmake/build/cmake/bgfx/shaderc";

    const bin_path_len = shader_path.len + 1;
    const bin_path = try b.allocator.alloc(u8, bin_path_len);
    @memmove(bin_path[0 .. bin_path_len - 1], shader_path);

    bin_path[bin_path_len - 3] = 'b';
    bin_path[bin_path_len - 2] = 'i';
    bin_path[bin_path_len - 1] = 'n';

    return b.addSystemCommand(&.{
        shaderc,
        "-f",
        shader_path,
        "-o",
        bin_path,
        "--type",
        &[_]u8{shader_type},
        "--platform",
        "osx",
        "--profile",
        "metal",
    });
}

fn buildShaders(b: *std.Build) !void {
    var shaders_directory = try std.fs.cwd().openDir("./resources/shaders/", .{ .access_sub_paths = false, .iterate = true });
    defer shaders_directory.close();

    var files = shaders_directory.iterate();
    while (try files.next()) |entry| {
        if (entry.kind != .file)
            continue;

        const extension = std.fs.path.extension(entry.name);
        const filename = std.fs.path.basename(entry.name);

        if (std.mem.eql(u8, extension, ".sc")) {
            const separtor_index = std.mem.lastIndexOfScalar(u8, filename, '_') orelse continue;
            const shader_type: u8 = filename[separtor_index + 1];

            if (shader_type != 'f' and shader_type != 'v') {
                continue;
            }

            var full_path_builder: std.ArrayList(u8) = .empty;
            defer full_path_builder.deinit(b.allocator);

            try full_path_builder.appendSlice(b.allocator, "./resources/shaders/");
            try full_path_builder.appendSlice(b.allocator, entry.name);

            const build_shader_step = try buildShader(b, full_path_builder.items, shader_type);
            b.default_step.dependOn(&build_shader_step.step);
        }
    }
}

fn buildLibraryCMake(b: *std.Build, comptime name: [:0]const u8) void {
    const source_directory = "vendor/" ++ name;
    const build_directory = source_directory ++ "/build";

    const cmake_step = b.addSystemCommand(&.{
        "cmake",
        "-S",
        source_directory,
        "-B",
        build_directory,
    });

    const cmake_build_step = b.addSystemCommand(&.{
        "cmake",
        "--build",
        build_directory,
        "-j16",
    });

    cmake_build_step.step.dependOn(&cmake_step.step);
    b.default_step.dependOn(&cmake_build_step.step);
}

pub fn addDependencyModule(
    b: *std.Build,
    comptime name: []const u8,
    target: std.Build.ResolvedTarget,
    optimize: std.builtin.OptimizeMode,
) *std.Build.Module {
    const dep = b.dependency(name, .{
        .target = target,
        .optimize = optimize,
    });

    return dep.module(name);
}

pub fn build(b: *std.Build) !void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    buildLibraryCMake(b, "SDL");
    buildLibraryCMake(b, "bgfx.cmake");

    try buildShaders(b);

    // fetch dependencies
    const imports: []const std.Build.Module.Import = &.{
        .{ .name = "zigimg", .module = addDependencyModule(b, "zigimg", target, optimize) },
        .{ .name = "zlm", .module = addDependencyModule(b, "zlm", target, optimize) },
        .{ .name = "fey_asset", .module = addDependencyModule(b, "fey_asset", target, optimize) },
        .{ .name = "fey_ecs", .module = addDependencyModule(b, "fey_ecs", target, optimize) },
    };

    // build fey
    const fey_module = b.addModule("fey", .{
        .root_source_file = b.path("src/lib.zig"),
        .target = target,
        .optimize = optimize,
        .imports = imports,
    });

    fey_module.addCMacro("BX_CONFIG_DEBUG", if (optimize == .Debug) "1" else "0");
    fey_module.addCMacro("BGFX_CONFIG_DEBUG", if (optimize == .Debug) "1" else "0");

    // add includes paths
    // SDL
    fey_module.addIncludePath(b.path("vendor/SDL/include/"));
    // BGFX
    fey_module.addIncludePath(b.path("vendor/bgfx.cmake/bgfx/include/"));
    fey_module.addIncludePath(b.path("vendor/bgfx.cmake/bimg/include/"));
    fey_module.addIncludePath(b.path("vendor/bgfx.cmake/bx/include/"));

    // link Frameworks and Liraries
    fey_module.linkFramework("CoreFoundation", .{});
    fey_module.linkFramework("CoreGraphics", .{});
    fey_module.linkFramework("ApplicationServices", .{});
    fey_module.linkFramework("Cocoa", .{});
    fey_module.linkFramework("IOKit", .{});
    fey_module.linkFramework("CoreAudio", .{});
    fey_module.linkFramework("CoreVideo", .{});
    fey_module.linkFramework("Metal", .{});
    fey_module.linkFramework("QuartzCore", .{});

    fey_module.linkSystemLibrary("stdc++", .{});

    fey_module.addLibraryPath(b.path("vendor/SDL/build/"));
    fey_module.linkSystemLibrary("SDL3", .{});

    fey_module.addLibraryPath(b.path("vendor/bgfx.cmake/build/cmake/bgfx"));
    fey_module.linkSystemLibrary("bgfx", .{});

    fey_module.addLibraryPath(b.path("vendor/bgfx.cmake/build/cmake/bimg"));
    fey_module.linkSystemLibrary("bimg", .{});
    fey_module.linkSystemLibrary("bimg_decode", .{});
    fey_module.linkSystemLibrary("bimg_encode", .{});

    fey_module.addLibraryPath(b.path("vendor/bgfx.cmake/build/cmake/bx"));
    fey_module.linkSystemLibrary("bx", .{});

    const fey_library = b.addLibrary(.{
        .name = "fey",
        .root_module = b.createModule(.{
            .root_source_file = b.path("src/lib.zig"),
            .target = b.graph.host,
        }),
    });
    b.installArtifact(fey_library);

    // build examples
    {
        const modern = b.addExecutable(.{
            .name = "main",
            .root_module = b.createModule(.{
                .root_source_file = b.path("examples/modern/main.zig"),
                .target = b.graph.host,
            }),
        });
        modern.root_module.addImport("fey", fey_module);

        b.installArtifact(modern);
    }
}
