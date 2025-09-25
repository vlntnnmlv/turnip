// Copyright 2025 Valentin Namleev

#pragma once

#include <SDL3/SDL.h>
#include <bgfx/bgfx.h>

namespace turnip {
class App {
public:
    App(const char *_title, int _width, int _height);
    virtual ~App();

    void Run();

protected:
    virtual void Update(bgfx::ProgramHandle _program);

protected:
    int m_width;
    int m_height;
    bool m_running;

private:
    SDL_Window *m_window{};
};
} // namespace turnip