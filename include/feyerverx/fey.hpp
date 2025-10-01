// Copyright 2025 Valentin Namleev

#pragma once

#include <memory>
#include <optional>
#include <string>

#include <SDL3/SDL.h>
#include <bgfx/platform.h>
#include <bx/math.h>

#include "feyerverx/camera.hpp"
#include "feyerverx/scene.hpp"

namespace feyerverx {
using unique_ptr_SDL_Window = std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)>;
#define MAKE_UNIQUE_SDL_WINDOW(...)                                                                \
    unique_ptr_SDL_Window(SDL_CreateWindow(__VA_ARGS__), SDL_DestroyWindow)

class Fey {
public:
    Fey(const std::string &title, int _width, int _height);
    ~Fey();

    void run();

private:
    void tryInitSDL();
    void tryInitBGFX();
    void initCamera();

private:
    std::string m_title;

    int m_width;
    int m_height;
    bool m_running;
    std::unique_ptr<ICamera> m_camera;
    unique_ptr_SDL_Window m_window;

    Scene m_scene;
};
} // namespace feyerverx