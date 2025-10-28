// Copyright 2025 Valentin Namleev

#include <chrono>
#include <print>
#include <utility>

#include <bgfx/platform.h>
#include <bx/math.h>

#include "feyerverx/events/event.hpp"
#include "feyerverx/fey.hpp"

namespace feyerverx {

std::expected<Fey, Error> Fey::create(const std::string &name, const float width,
                                      const float height) {
    return create(Specification{name, {width, height}});
}

std::expected<Fey, Error> Fey::create(const Specification &specification) {
    auto guardSDLResult = GuardSDL::create(specification.name, specification.windowSize.x,
                                           specification.windowSize.y);
    if (!guardSDLResult) {
        return std::unexpected{guardSDLResult.error()};
    }
    unique_ptr_guard_sdl guardSDL = std::move(guardSDLResult.value());

    auto nativeWindowHandleResult = guardSDL->windowHandle();
    if (!nativeWindowHandleResult) {
        return std::unexpected{nativeWindowHandleResult.error()};
    }
    void *nativeWindowHandle = nativeWindowHandleResult.value();

    auto guardBGFXResult = GuardBGFX::create(nativeWindowHandle, specification.windowSize.x,
                                             specification.windowSize.y);
    if (!guardBGFXResult.value()) {
        return std::unexpected{guardBGFXResult.error()};
    }
    unique_ptr_guard_bgfx guardBGFX = std::move(guardBGFXResult.value());

    return Fey{specification, std::move(guardSDL), std::move(guardBGFX)};
}

Fey::Fey(Fey &&other) noexcept
    : m_specification{std::move(other.m_specification)}, m_guardSDL{std::move(other.m_guardSDL)},
      m_guardBGFX{std::move(other.m_guardBGFX)}, m_assetManager(std::move(other.m_assetManager)),
      m_renderSystem(std::move(other.m_renderSystem)), m_scenes(std::move(other.m_scenes)) {}

Fey::Fey(Specification spec, unique_ptr_guard_sdl &&guardSDL, unique_ptr_guard_bgfx &&guardBGFX)
    : m_specification{std::move(spec)}, m_guardSDL{std::move(guardSDL)},
      m_guardBGFX{std::move(guardBGFX)} {}

// getters
Specification &Fey::specification() { return m_specification; }
AssetManager &Fey::assetManager() { return m_assetManager; }
EventManager &Fey::eventManager() { return m_eventManager; }

// modifiers
ecs::Scene &Fey::addScene(const std::string &id, RectangleOffset viewport, Color backgroundColor,
                          const bool isActive) {
    ecs::Scene &scene = m_scenes.emplace_back(id, m_eventManager, viewport, backgroundColor);
    scene.isActive = isActive;
    return scene;
}

void Fey::run() {
    m_running = true;
    m_eventManager.queueEvent<WindowResizedEvent>(m_specification.windowSize);

    while (m_running) {
        processEvents();
        update();
        render();
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

            m_eventManager.queueEvent<WindowResizedEvent>(m_specification.windowSize);

            bgfx::reset(newWidth, newHeight, BGFX_RESET_VSYNC);
        }

        if (event.type == SDL_EVENT_KEY_DOWN) {
            if (event.key.key == SDLK_ESCAPE)
                m_running = false;

            m_eventManager.queueEvent<KeyboardEvent>(event.key.key, KeyboardEvent::Type::PRESSED);
        }
    }

    m_eventManager.dispatch();
}

void Fey::update() {
    m_clock.update();

    const float deltaTime = m_clock.deltaTimeSeconds();

    for (const ecs::Scene &scene : m_scenes) {
        if (!scene.isActive)
            continue;

        scene.update(deltaTime);
    }
}

void Fey::render() {
    bgfx::touch(0);
    for (auto it = m_scenes.begin(); it != m_scenes.end(); ++it) {
        ecs::Scene &scene = *it;

        if (!scene.isActive)
            continue;

        const uint16_t viewID = std::distance(m_scenes.begin(), it);
        m_renderSystem.render(scene, viewID);
    }

    bgfx::setDebug(BGFX_DEBUG_TEXT);
    bgfx::dbgTextClear();
    bgfx::dbgTextPrintf(1, 1, 0x4f, "elapsed: %fs", m_clock.elapsedTimeSeconds());
    bgfx::dbgTextPrintf(1, 2, 0x4f, "dt: %fs", m_clock.deltaTimeSeconds());
    bgfx::dbgTextPrintf(1, 3, 0x4f, "fps: %f", 1 / m_clock.deltaTimeSeconds());

    bgfx::frame();
}
} // namespace feyerverx