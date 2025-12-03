// Copyright 2025 Valentin Namleev

#include "feyerverx/guardSDL.hpp"
#include "feyerverx/error.hpp"
#include "feyerverx/logger.hpp"

namespace feyerverx {
std::expected<std::unique_ptr<GuardSDL>, Error>
GuardSDL::create(const std::string &title, const float width, const float height) {
    if (!SDL_Init(0)) {
        return std::unexpected{Error{ErrorType::SDLInitializationError, SDL_GetError()}};
    }

    SDL_Window *window =
        SDL_CreateWindow(title.c_str(), static_cast<int>(width), static_cast<int>(height),
                         SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY);

    if (!window) {
        return std::unexpected{Error{ErrorType::WindowInitializationError, SDL_GetError()}};
    }

    Logger::instance().log(LogLevel::Info, "Initialized SDL!");

    return std::unique_ptr<GuardSDL>(new GuardSDL{window});
}

GuardSDL::GuardSDL(GuardSDL &&other) noexcept : m_window{std::move(other.m_window)} {}

GuardSDL &GuardSDL::operator=(GuardSDL &&other) noexcept {
    if (this != &other) {
        m_window = std::move(other.m_window);
    }

    return *this;
}

GuardSDL::GuardSDL(SDL_Window *window) : m_window{window, SDL_DestroyWindow} {
    Logger::instance().log(LogLevel::Info, "GuardSDL constructed!");
}

GuardSDL::~GuardSDL() {
    SDL_Quit();
    Logger::instance().log(LogLevel::Info, "GuardSDL destroyed!");
}

std::expected<void *, Error> GuardSDL::windowHandle() const {
    const SDL_PropertiesID windowProperties = SDL_GetWindowProperties(m_window.get());

    // TODO: Add cross-platform logic of getting window handle:
    // https://wiki.libsdl.org/SDL3/SDL_GetWindowProperties
    void *nativeWindowHandle =
        SDL_GetPointerProperty(windowProperties, SDL_PROP_WINDOW_COCOA_WINDOW_POINTER, nullptr);
    if (!nativeWindowHandle) {
        return std::unexpected{Error{ErrorType::GettingNativeWindowHandleError, SDL_GetError()}};
    }

    return nativeWindowHandle;
}
}