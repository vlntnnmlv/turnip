// Copyright 2025 Valentin Namleev

#include <bgfx/platform.h>
#include <bx/timer.h>
#include <chrono>
#include <iostream>
#include <print>
#include <utility>

#include "feyerverx/fey.hpp"
#include "feyerverx/logger.hpp"

namespace feyerverx {
std::variant<Fey, Error> Fey::create(const std::string &title, const float width,
                                     const float height) {
    std::print("1\n");
    auto guardSDL = GuardSDL::create(title, width, height);
    std::print("2\n");

    std::print("3\n");
    if (std::holds_alternative<Error>(guardSDL))
        return std::get<Error>(guardSDL);
    std::print("4\n");

    auto nativeWindowHandle = std::get<std::unique_ptr<GuardSDL>>(guardSDL)->windowHandle();
    if (std::holds_alternative<Error>(nativeWindowHandle))
        return std::get<Error>(nativeWindowHandle);

    auto guardBGFX = GuardBGFX::create(std::get<void *>(nativeWindowHandle), width, height);

    if (std::holds_alternative<Error>(guardBGFX))
        return std::get<Error>(guardBGFX);

    return Fey{title, width, height, std::get<std::unique_ptr<GuardSDL>>(guardSDL),
               std::get<std::unique_ptr<GuardBGFX>>(guardBGFX)};
}

Fey::Fey(Fey &&other) noexcept
    : m_title{std::move(other.m_title)}, m_width{other.m_width}, m_height{other.m_height},
      m_guardSDL{std::move(other.m_guardSDL)}, m_guardBGFX{std::move(other.m_guardBGFX)} {}

Fey::Fey(std::string title, const float width, const float height,
         std::unique_ptr<GuardSDL> &guardSDL, std::unique_ptr<GuardBGFX> &guardBGFX)
    : m_title(std::move(title)), m_width(width), m_height(height), m_guardSDL(std::move(guardSDL)),
      m_guardBGFX(std::move(guardBGFX)) {
    m_globalSystems.push_back(std::make_unique<ecs::RenderSystem>());
}

void Fey::initCamera() {
    m_camera = std::make_unique<CameraOrthogonal>(0.0f, m_width, 0.0f, m_height);
    m_camera->setView();

    Logger::instance().log(LogLevel::Info, "Inited camera!");
}

AssetManager &Fey::assetManager() { return m_assetManager; }

ecs::Scene &Fey::addScene(const std::string &id, const bool isActive) {
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

        if (event.type == SDL_EVENT_WINDOW_RESIZED) {
            const int newWidth = event.window.data1;
            const int newHeight = event.window.data2;

            m_width = static_cast<float>(newWidth);
            m_height = static_cast<float>(newHeight);

            m_camera->resizeView(m_width, m_height);
            m_camera->setView();
            bgfx::reset(newWidth, newHeight, BGFX_RESET_VSYNC);
        }

        if (event.type == SDL_EVENT_KEY_DOWN) {
            if (event.key.key == SDLK_ESCAPE)
                m_running = false;
        }

        for (ecs::Scene &scene : m_scenes) {
            if (!scene.isActive)
                continue;

            scene.enqueueEvent(event);
        }
    }
}

void Fey::update() {
    m_clock.update();

    const float deltaTime = m_clock.deltaTimeSeconds();
    for (ecs::Scene &scene : m_scenes) {
        if (!scene.isActive)
            continue;

        scene.update(deltaTime);
        for (const std::unique_ptr<ecs::ISystem> &system : m_globalSystems) {
            system->enqueueScene(scene);
        }
    }

    for (std::unique_ptr<ecs::ISystem> &system : m_globalSystems) {
        system->update(deltaTime);
    }
}
} // namespace feyerverx