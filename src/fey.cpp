// Copyright 2025 Valentin Namleev

#include <bgfx/platform.h>
#include <bx/math.h>
#include <bx/timer.h>
#include <chrono>
#include <format>
#include <iostream>
#include <print>

#include "feyerverx/error.hpp"
#include "feyerverx/fey.hpp"
#include "feyerverx/logger.hpp"

namespace feyerverx {
Fey::Fey(const std::string &title, int width, int height)
    : m_title(std::move(title)), m_width(width), m_height(height),
      m_window(nullptr, &SDL_DestroyWindow) {

    tryInitSDL();
    tryInitBGFX();
    initCamera();

    m_scene.init();
}

void Fey::tryInitSDL() {
    if (!SDL_Init(0)) {
        throw FeyError(FeyErrorType::SDLInitializationError, SDL_GetError());
    }

    m_window = MAKE_UNIQUE_SDL_WINDOW(m_title.c_str(), m_width, m_height,
                                      SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY);

    if (!m_window) {
        throw FeyError(FeyErrorType::WindowInitializationError, SDL_GetError());
    }

    Logger::instance().log(LogLevel::Info, "Inited SDL!");
}

void Fey::tryInitBGFX() {
    bgfx::renderFrame();

    bgfx::Init init;
    init.type = bgfx::RendererType::Count;
    SDL_PropertiesID windowProperties = SDL_GetWindowProperties(m_window.get());

    // TODO: Add crossplatfom logic of getting window handle:
    // https://wiki.libsdl.org/SDL3/SDL_GetWindowProperties
    void *nativeWindowHandle =
        SDL_GetPointerProperty(windowProperties, SDL_PROP_WINDOW_COCOA_WINDOW_POINTER, nullptr);
    if (!nativeWindowHandle) {
        throw FeyError(FeyErrorType::GettingNativeWindowHandleError, SDL_GetError());
    }

    init.platformData.nwh = nativeWindowHandle;
    init.resolution.width = m_width;
    init.resolution.height = m_height;
    init.resolution.reset = BGFX_RESET_VSYNC;

    init.platformData.ndt = nullptr;
    init.platformData.context = nullptr;
    init.platformData.backBuffer = nullptr;
    init.platformData.backBufferDS = nullptr;

    // On Apple's macOS, you must set the NSHighResolutionCapable Info.plist property to YES,
    // otherwise you will not receive a High-DPI OpenGL canvas.

    if (!bgfx::init(init)) {
        throw FeyError(FeyErrorType::BGFXInitializationError, "BGFX failed to initialize");
    }

    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff);
    bgfx::frame();

    Logger::instance().log(LogLevel::Info, "Inited BGFX!");
}

void Fey::initCamera() {
    m_camera = std::make_unique<CameraOrthogonal>(0.0f, (float)m_width, 0.0f, (float)m_height);
    m_camera->setView();

    Logger::instance().log(LogLevel::Info, "Inited camera!");
}

void Fey::run() {
    m_running = true;
    while (m_running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {

            if (event.type == SDL_EVENT_QUIT) {
                m_running = false;
            }

            if (event.type == SDL_EVENT_KEY_DOWN) {
                if (event.key.key == SDLK_ESCAPE)
                    m_running = false;

                if (event.type == SDL_EVENT_WINDOW_RESIZED) {
                    int newW = event.window.data1;
                    int newH = event.window.data2;

                    m_width = newW;
                    m_height = newH;

                    bgfx::reset(uint32_t(m_width), uint32_t(m_height), BGFX_RESET_VSYNC);
                    m_camera->resizeView(m_width, m_height);
                    m_camera->setView();
                }

                m_scene.processEvent(event);
            }

            bgfx::touch(0);
            bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff);

            m_scene.update();

            bgfx::setDebug(BGFX_DEBUG_TEXT);
            bgfx::dbgTextClear();
            bgfx::dbgTextPrintf(1, 1, 0x4f, "Fey running");

            bgfx::frame();
        }
    }
}

Fey::~Fey() {
    // bgfx::shutdown(); TODO: Members are destroyed after this destructor, so if we call it later,
    // we cant free resources in renderer
    SDL_Quit();
    Logger::instance().log(LogLevel::Info, "Fey destroyed!");
}
} // namespace feyerverx