![Turnip](resources/textures/turnip.png)

# Turnip

C++ ECS Game engine

This project's goal is to undestand how big ECS system's work under the hood.

**WORK IN PROGRESS**

## Dependencies

[raylib](https://github.com/raysan5/raylib) and [raylib-cpp](https://github.com/RobLoach/raylib-cpp)

## Somewhat implemented features

- ECS (Entity, Component, System)
- Components for simple UI (Image, Text, Button, etc.)
- Automatic layouting with panels and stacks
- Animation(update) system *in progress*
- Input system supporting hovering and pressing any entity with corresponding components

## Build

* ```make``` in root directory to build a static library (you will need raylib and raylib-cpp headers)
* compile your project the way you like, just include a raylib static library

The project has only been build on macOS, and there no convinient way of setting up a build. But if you know what you are doing you will get it. However, work is beeing done, and it's gonna be better soon.

## Examples
Look at the examples code to get the idea of how ECS is used in a real project (probably not the best pratices are used in that example xD)

