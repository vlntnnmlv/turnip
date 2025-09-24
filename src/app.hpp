// Copyright 2025 Valentin Namleev

#pragma once

#include <SDL3/SDL.h>
#include <bgfx/platform.h>
#include <iostream>

class App {
public:
    App(const char *_title, int _width, int _height);
    virtual ~App();

    virtual void Update();
    void Run();

protected:
    int m_width;
    int m_height;
    bool m_running;

private:
    SDL_Window *m_window{};
};