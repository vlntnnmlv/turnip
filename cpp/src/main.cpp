// Copyright 2025 Valentin Namleev

#include <raylib-cpp.hpp>
#include "./engine.hpp"
#include "./node.hpp"

int main() {
    Turnip::Engine engine = Turnip::Engine(800, 450, "Turnip");
    auto a = Turnip::Node::Create("FFF");
    auto b = Turnip::Node::Create("Fdd", a);

    engine.Run();

    // UnloadTexture() and CloseWindow() are called automatically.

    return 0;
}
