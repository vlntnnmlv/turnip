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
      m_guardSDL(m_title, m_width, m_height),
      m_guardBGFX(m_guardSDL.windowHandle(), m_width, m_height) {

    initCamera();

    m_scene.init();
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

Fey::~Fey() {}
} // namespace feyerverx