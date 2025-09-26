// Copyright 2025 Valentin Namleev

#pragma once

#include <memory>
#include <string>

#include <SDL3/SDL.h>
#include <bgfx/platform.h>
#include <bx/math.h>

#include "turnip/camera.hpp"

using SDLWindowUniquePtr = std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)>;
#define MakeUniqueSDLWindow(...)                                                                   \
    SDLWindowUniquePtr(SDL_CreateWindow(__VA_ARGS__), SDL_DestroyWindow)

namespace turnip {

class App {
public:
    App(const char *_title, int _width, int _height);
    virtual ~App();

    void Run();

protected:
    virtual void Update();
    virtual void ProcessEvent([[maybe_unused]] const SDL_Event &_event);

private:
    void initSDL();
    void initBGFX();

protected:
    std::string m_title;
    int m_width;
    int m_height;
    bool m_running;
    std::unique_ptr<ICamera> m_camera;

private:
    SDLWindowUniquePtr m_window;
};
} // namespace turnip