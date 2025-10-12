// Copyright 2025 Valentin Namleev

#include <bgfx/platform.h>
#include <bx/timer.h>
#include <chrono>
#include <iostream>
#include <print>
#include <utility>

#include "feyerverx/fey.hpp"

#include "feyerverx/eventHandler.hpp"
#include "feyerverx/logger.hpp"

namespace feyerverx {

std::expected<Fey, Error> Fey::create(const std::string &title, const float width,
                                      const float height) {
    auto guardSDLResult = GuardSDL::create(title, width, height);
    if (!guardSDLResult) {
        return std::unexpected{guardSDLResult.error()};
    }
    unique_ptr_guard_sdl guardSDL = std::move(guardSDLResult.value());

    auto nativeWindowHandleResult = guardSDL->windowHandle();
    if (!nativeWindowHandleResult) {
        return std::unexpected{nativeWindowHandleResult.error()};
    }
    void *nativeWindowHandle = nativeWindowHandleResult.value();

    auto guardBGFXResult = GuardBGFX::create(nativeWindowHandle, width, height);
    if (!guardBGFXResult.value()) {
        return std::unexpected{guardBGFXResult.error()};
    }
    unique_ptr_guard_bgfx guardBGFX = std::move(guardBGFXResult.value());

    auto camera = CameraOrthogonal::createUniquePointer(0.0f, width, 0.0f, height);

    return Fey{Specification{title, Vector2f{width, height}}, std::move(guardSDL),
               std::move(guardBGFX), std::move(camera)};
}

Fey::Fey(Fey &&other) noexcept
    : m_specification(std::move(other.m_specification)), m_guardSDL(std::move(other.m_guardSDL)),
      m_guardBGFX(std::move(other.m_guardBGFX)), m_camera(std::move(other.m_camera)) {}

Fey::Fey(Specification spec, unique_ptr_guard_sdl &&guardSDL, unique_ptr_guard_bgfx &&guardBGFX,
         std::unique_ptr<ICamera> &&camera)
    : m_specification(std::move(spec)), m_guardSDL(std::move(guardSDL)),
      m_guardBGFX(std::move(guardBGFX)), m_camera(std::move(camera)) {
    int i = 0;
}

AssetManager &Fey::assetManager() { return m_assetManager; }

Specification &Fey::specification() { return m_specification; }

void Fey::initGlobalSystems() {
    Logger::instance().log(LogLevel::Info, "Initializing global systems...");
    m_globalSystems.push_back(std::make_unique<ecs::RenderSystem>());
}

ecs::Scene &Fey::addScene(const std::string &id, const bool isActive) {
    ecs::Scene &scene = m_scenes.emplace_back(id);
    scene.isActive = isActive;
    return scene;
}

void Fey::run() {
    m_camera->setView();

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

            m_specification.windowSize.x = static_cast<float>(newWidth);
            m_specification.windowSize.y = static_cast<float>(newHeight);

            m_camera->resizeView(m_specification.windowSize.y, m_specification.windowSize.x);
            m_camera->setView();
            bgfx::reset(newWidth, newHeight, BGFX_RESET_VSYNC);
        }

        if (event.type == SDL_EVENT_KEY_DOWN) {
            EventSink::OnKeyDown.invoke(event.key.key);

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