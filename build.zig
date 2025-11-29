// 1. build bgfx and sdl
// 2. build fey
// 3. build examples

const std = @import("std");

pub fn build(b: *std.Build) void {
    // build SDL
    {
        const SDL_cmake = b.addSystemCommand(&.{
            "cmake",
            "-S",
            "vendor/SDL",
            "-B",
            "vendor/SDL/build",
        });

        const SDL_cmake_build = b.addSystemCommand(&.{
            "cmake",
            "--build",
            "vendor/SDL/build",
            "-j16",
        });

        SDL_cmake_build.step.dependOn(&SDL_cmake.step);
        b.default_step.dependOn(&SDL_cmake_build.step);
    }

    // build BGFX
    {
        const bgfx_cmake = b.addSystemCommand(&.{
            "cmake",
            "-S",
            "vendor/bgfx.cmake",
            "-B",
            "vendor/bgfx.cmake/build",
        });

        const bgfx_cmake_build = b.addSystemCommand(&.{
            "cmake",
            "--build",
            "vendor/bgfx.cmake/build",
            "-j16",
        });

        bgfx_cmake_build.step.dependOn(&bgfx_cmake.step);
        b.default_step.dependOn(&bgfx_cmake_build.step);
    }

    // build fey
    // const fey_library = b.addLibrary(.{
    //     .name = "fey",
    //     .root_module = b.createModule(.{
    //         .root_source_file = b.path("src/main.zig"),
    //         .target = b.graph.host,
    //     }),
    // });

    const fey_module = b.addModule("fey", .{
        .root_source_file = b.path("src/main.zig"),
    });
    fey_module.addIncludePath(b.path("vendor/SDL/include/"));
    fey_module.addIncludePath(b.path("vendor/bgfx.cmake/bgfx/include/"));
    fey_module.addIncludePath(b.path("vendor/bgfx.cmake/bimg/include/"));
    fey_module.addIncludePath(b.path("vendor/bgfx.cmake/bx/include/"));

    // b.installArtifact(fey_library);

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

        modern.root_module.linkFramework("CoreFoundation", .{});
        modern.root_module.linkFramework("CoreGraphics", .{});
        modern.root_module.linkFramework("ApplicationServices", .{});
        modern.root_module.linkFramework("Cocoa", .{});
        modern.root_module.linkFramework("IOKit", .{});
        modern.root_module.linkFramework("CoreAudio", .{});
        modern.root_module.linkFramework("CoreVideo", .{});
        modern.root_module.linkFramework("Metal", .{});
        modern.root_module.linkFramework("QuartzCore", .{});
        modern.root_module.linkSystemLibrary("stdc++", .{});

        modern.root_module.addLibraryPath(b.path("vendor/SDL/build/"));
        modern.root_module.linkSystemLibrary("SDL3", .{});

        modern.root_module.addLibraryPath(b.path("vendor/bgfx.cmake/build/cmake/bgfx"));
        modern.root_module.linkSystemLibrary("bgfx", .{});

        modern.root_module.addLibraryPath(b.path("vendor/bgfx.cmake/build/cmake/bimg"));
        modern.root_module.linkSystemLibrary("bimg", .{});
        modern.root_module.linkSystemLibrary("bimg_decode", .{});
        modern.root_module.linkSystemLibrary("bimg_encode", .{});

        modern.root_module.addLibraryPath(b.path("vendor/bgfx.cmake/build/cmake/bx"));
        modern.root_module.linkSystemLibrary("bx", .{});

        b.installArtifact(modern);
    }
}
