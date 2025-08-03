// Copyright 2025 Valentin Namleev

#pragma once

#include <string>
#include <iostream>

#include <Vector2.hpp>
#include <Window.hpp>

namespace Turnip {
class Engine {
 public:
    Engine(float _WindowWidth, float _WindowHeight, const std::string& _WindowTitle) {
        m_Size = { _WindowWidth, _WindowHeight };

        InitWindow(_WindowTitle);
    }

    ~Engine() { }

    void Run() {
        while (!m_Window->ShouldClose()) {
            Update();
            Render();
        }
    }

 private:
    void InitWindow(const std::string& _WindowTitle) {
        unsigned int windowFlags = FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI | FLAG_WINDOW_RESIZABLE;
        m_Window = new raylib::Window(m_Size.x, m_Size.y, _WindowTitle, windowFlags);
    }

    void Update() { }

    void Render() {
        BeginDrawing();
        m_Window->ClearBackground(RAYWHITE);
        DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);
        EndDrawing();
    }

 private:
    Vector2 m_Size;
    raylib::Window* m_Window;
};
}  // namespace Turnip
