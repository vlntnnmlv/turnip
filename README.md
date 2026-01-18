# Fey

ECS game engine in Zig (WIP)

Fey is a small experimental engine built around an Entity–Component–System architecture, SDL3 for window/input, and bgfx for rendering. It currently targets macOS/Metal.

Status: active work-in-progress. APIs may change.


## Features (current)

- ECS core: entity registry, components as plain Zig structs, simple systems API
- SDL3 backend for windowing/input
- bgfx renderer scaffold with shader build integration
- Asset loading for images and fonts (via zigimg and stb)
- Example app under examples/modern


## Prerequisites

- macOS with Xcode Command Line Tools (for compilers, headers)
- Zig 0.13+ (recommended)
- CMake 3.24+ (for building SDL and bgfx)

The build script targets Metal (osx). Linux/Windows are not wired yet.


## Getting started

Clone with submodules:

- git clone --recurse-submodules https://github.com/<your-username>/fey.git
  - If you already cloned: git submodule update --init --depth 1

Build (first build will compile SDL, bgfx and shaderc, so it takes longer):

- zig build

Run the example (run from repo root so relative asset paths resolve):

- ./zig-out/bin/main

Common options:

- zig build -Doptimize=ReleaseFast
- zig build -Dtarget=aarch64-macos.13.0

The build will also compile all resources/shaders/*.sc into .bin files using bgfx shaderc.


## How assets and shaders are loaded

- Shaders: .sc files in resources/shaders are compiled at build time to .bin alongside the sources; the engine loads the .bin at runtime.
- Textures/Fonts: loaded from resources/textures and resources/fonts. Keep the working directory at the repo root when running to resolve relative paths.


## ECS overview

- Components are plain Zig structs. The lowercased type name is used as a field key in component views.
- The Registry is responsible for creating/removing entities and attaching/detaching components.
- Systems iterate over views (e.g., entities having components A and B) and perform logic.

See:
- src/ecs/registry.zig and src/ecs/systems.zig
- examples/modern/main.zig for usage


## Troubleshooting

- Missing CMake: install via brew install cmake
- Xcode tools not found: xcode-select --install
- First build is slow: it builds SDL, bgfx, and shaderc; subsequent builds are much faster
- Binary cannot find resources: run from the repository root so relative paths to ./resources resolve
- Shader compiler not found: ensure submodules are initialized; the build generates vendor/bgfx.cmake/build/cmake/bgfx/shaderc


## Roadmap (short)

- Cross-platform build targets (Windows/Linux)
- Input/event channel and keybinding map
- System scheduler and fixed timestep helper
- Debug overlay (FPS, draw calls) and logging facade


## License

TBD
