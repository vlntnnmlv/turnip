// Copyright 2025 Valentin Namleev

#include <bgfx/platform.h>
#include <bx/math.h>
#include <bx/timer.h>
#include <format>
#include <iostream>

#include "turnip/app.hpp"

namespace turnip {
App::App(const char *title, int width, int height)
    : m_title(title), m_width(width), m_height(height), m_window(nullptr, SDL_DestroyWindow) {
    initSDL();
    initBGFX();

    m_camera = std::make_unique<CameraOrthogonal>(0.0f, (float)m_width, 0.0f, (float)m_height);
    m_camera->setView();

    m_running = true;
}

void App::initSDL() {
    if (!SDL_Init(0)) {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << "\n";
        throw new std::runtime_error(std::format("SDL_Init failed: {}", SDL_GetError()));
    }

    m_window = MakeUniqueSDLWindow(m_title.c_str(), m_width, m_height,
                                   SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY);

    if (!m_window) {
        throw new std::runtime_error(std::format("SDL_CreateWindow failed: {}", SDL_GetError()));
    }
}

void App::initBGFX() {
    bgfx::renderFrame();

    bgfx::Init init;
    init.type = bgfx::RendererType::Count;
    SDL_PropertiesID windowProperties = SDL_GetWindowProperties(m_window.get());

    // TODO: Add crossplatfom logic of getting window handle:
    // https://wiki.libsdl.org/SDL3/SDL_GetWindowProperties
    void *nativeWindowHandle =
        SDL_GetPointerProperty(windowProperties, SDL_PROP_WINDOW_COCOA_WINDOW_POINTER, nullptr);
    if (!nativeWindowHandle) {
        throw std::runtime_error("Couldn't get native window handle!");
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
        throw std::runtime_error("bgfx::init failed!");
    }

    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff);
    bgfx::frame();
}

void App::Run() {
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

                ProcessEvent(event);
            }

            bgfx::touch(0);
            bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff);

            Update();

            bgfx::setDebug(BGFX_DEBUG_TEXT);
            bgfx::dbgTextClear();
            bgfx::dbgTextPrintf(1, 1, 0x4f, "Turnip running");

            bgfx::frame();
        }
    }
}

void App::Update() {}

void App::ProcessEvent(const SDL_Event &_event) {}

App::~App() {
    bgfx::shutdown();
    SDL_Quit();
}
} // namespace turnip