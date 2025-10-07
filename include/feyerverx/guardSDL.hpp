// Copyright 2025 Valentin Namleev

#pragma once

#include <memory>
#include <string>

#include <SDL3/SDL.h>

namespace feyerverx {
using unique_ptr_SDL_Window = std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)>;
#define MAKE_UNIQUE_SDL_WINDOW(...)                                                                \
    unique_ptr_SDL_Window(SDL_CreateWindow(__VA_ARGS__), SDL_DestroyWindow)

class GuardSDL {
public:
    GuardSDL(const std::string &title, int width, int height);
    ~GuardSDL();
    [[nodiscard]] void *windowHandle() const;

private:
    unique_ptr_SDL_Window m_window;
};
} // namespace turnip
