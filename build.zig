// 1. build bgfx and sdl
// 2. build fey
// 3. build examples

const std = @import("std");

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

pub fn build(b: *std.Build) void {
    buildLibraryCMake(b, "SDL");
    buildLibraryCMake(b, "bgfx.cmake");

    // build fey
    const fey_module = b.addModule("fey", .{
        .root_source_file = b.path("src/main.zig"),
        .target = b.standardTargetOptions(.{}),
    });

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
            .root_source_file = b.path("src/main.zig"),
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
