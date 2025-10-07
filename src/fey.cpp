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

    m_globalSystems.push_back(std::make_unique<ecs::RenderSystem>());
}

void Fey::initCamera() {
    m_camera = std::make_unique<CameraOrthogonal>(0.0f, (float)m_width, 0.0f, (float)m_height);
    m_camera->setView();

    Logger::instance().log(LogLevel::Info, "Inited camera!");
}

AssetManager &Fey::assetManager() { return m_assetManager; }

ecs::Scene &Fey::addScene(const std::string &id, bool isActive) {
    ecs::Scene &scene = m_scenes.emplace_back(id);
    scene.isActive = isActive;
    return scene;
}

void Fey::run() {
    m_running = true;
    while (m_running) {
        processEvents();
        update();

        bgfx::touch(0);
        bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff);

        // draw

        bgfx::setDebug(BGFX_DEBUG_TEXT);
        bgfx::dbgTextClear();
        bgfx::dbgTextPrintf(1, 1, 0x4f, "elapsed: %fs", m_clock.elapsedTimeSeconds());
        bgfx::dbgTextPrintf(1, 2, 0x4f, "dt: %fs", m_clock.deltaTimeSeconds());
        bgfx::dbgTextPrintf(1, 3, 0x4f, "fps: %f", 1 / m_clock.deltaTimeSeconds());
        bgfx::frame();
    }
}

void Fey::processEvents() {
    static SDL_Event event;
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

            for (ecs::Scene &scene : m_scenes) {
                if (!scene.isActive)
                    continue;

                scene.enqueueEvent(event);
            }
        }
    }
}

void Fey::update() {
    m_clock.update();

    float deltaTime = m_clock.deltaTimeSeconds();
    for (ecs::Scene &scene : m_scenes) {
        if (!scene.isActive)
            continue;

        scene.update(deltaTime);
        for (std::unique_ptr<ecs::ISystem> &system : m_globalSystems) {
            system->enqueueScene(scene);
        }
    }

    for (std::unique_ptr<ecs::ISystem> &system : m_globalSystems) {
        system->update(deltaTime);
    }
}

Fey::~Fey() {}
} // namespace feyerverx