// Copyright 2025 Valentin Namleev

#pragma once

#include "error.hpp"

#include <expected>
#include <memory>
#include <string>
#include <variant>

#include <SDL3/SDL.h>

namespace feyerverx {
using unique_ptr_SDL_Window = std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)>;

class GuardSDL {
public:
    static std::expected<std::unique_ptr<GuardSDL>, Error> create(const std::string &title,
                                                                  float width, float height);

    GuardSDL(const GuardSDL &) noexcept = delete;
    GuardSDL &operator=(const GuardSDL &) noexcept = delete;

    GuardSDL(GuardSDL &&) noexcept;
    GuardSDL &operator=(GuardSDL &&) noexcept;

    ~GuardSDL();

    [[nodiscard]] std::expected<void *, Error> windowHandle() const;

    unique_ptr_SDL_Window m_window;

private:
    explicit GuardSDL(SDL_Window *window);
};
} // namespace turnip
