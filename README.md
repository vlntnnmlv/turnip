# Fey

C++ ECS Game engine

At first this project's goal was just to undestand how big ECS system's work under the hood. But after about 50 hours of development time, I became interested in somewhat low-level graphics API's for complete control of the visuals.

**WORK IN PROGRESS**

## Dependencies

Rendering: [bgfx](https://github.com/bkaradzic/bgfx.git)

Windowing and events: [SDL](https://github.com/libsdl-org/SDL.git)

Images loading: [STB](https://github.com/nothings/stb)

The project was only built for OSX, but usage of cross-platform libraries like SDL, STB and BGFX imply that at some point **turnip** will be able to be built on every system.

## Somewhat implemented features

- ECS (Entity, Component, System)
- Components for simple UI (Image, Text, Button, etc.)
- Automatic layouting with panels and stacks
- Animation(update) system *in progress*
- Input system supporting hovering and pressing any entity with corresponding components

## Build

* ```cmake -S . -B build``` in root directory to generate cmake build directory
* Then ```cmake --build build```. You will find everything in build directory.
