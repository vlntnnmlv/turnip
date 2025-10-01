// Copyright 2025 Valentin Namleev

#include "feyerverx/guardSDL.hpp"
#include "feyerverx/error.hpp"
#include "feyerverx/logger.hpp"

namespace feyerverx {
GuardSDL::GuardSDL(const std::string &title, int width, int height)
    : m_window(nullptr, &SDL_DestroyWindow) {
    if (!SDL_Init(0)) {
        throw FeyError(FeyErrorType::SDLInitializationError, SDL_GetError());
    }

    m_window = MAKE_UNIQUE_SDL_WINDOW(title.c_str(), width, height,
                                      SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY);

    if (!m_window) {
        throw FeyError(FeyErrorType::WindowInitializationError, SDL_GetError());
    }

    Logger::instance().log(LogLevel::Info, "Inited SDL!");
}

GuardSDL::~GuardSDL() { SDL_Quit(); }

void *GuardSDL::windowHandle() const {
    SDL_PropertiesID windowProperties = SDL_GetWindowProperties(m_window.get());

    // TODO: Add crossplatfom logic of getting window handle:
    // https://wiki.libsdl.org/SDL3/SDL_GetWindowProperties
    void *nativeWindowHandle =
        SDL_GetPointerProperty(windowProperties, SDL_PROP_WINDOW_COCOA_WINDOW_POINTER, nullptr);
    if (!nativeWindowHandle) {
        throw FeyError(FeyErrorType::GettingNativeWindowHandleError, SDL_GetError());
    }

    return nativeWindowHandle;
}
}