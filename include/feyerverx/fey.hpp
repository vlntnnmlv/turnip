// Copyright 2025 Valentin Namleev

#pragma once

#include <list>
#include <memory>
#include <string>

#include "feyerverx/events/eventManager.hpp"

#include "feyerverx/ecs/scene.hpp"
#include "feyerverx/ecs/systems/renderSystem.hpp"

#include "feyerverx/clock.hpp"
#include "feyerverx/guardBGFX.hpp"
#include "feyerverx/guardSDL.hpp"
#include "feyerverx/specification.hpp"

// new Fey
// - new SDL
// - new BGFX
// - resources ...
// ---
// ~ resources ...
// ~ BGFX
// ~ SDL
// ~ Fey

namespace feyerverx {

using unique_ptr_guard_sdl = std::unique_ptr<GuardSDL>;
using unique_ptr_guard_bgfx = std::unique_ptr<GuardBGFX>;
using unique_ptr_system = std::unique_ptr<ecs::ISystem>;

class Fey {
public:
    static Fey create(const std::string &name, float width, float height);
    static Fey create(const Specification &specification);

    Fey(const Fey &fey) = delete;
    Fey &operator=(const Fey &fey) = delete;
    Fey(Fey &&other) noexcept;
    Fey &operator=(Fey &&other) = delete;

    ~Fey() = default;

    // getters
    Specification &specification();
    AssetManager &assetManager();
    EventManager &eventManager();

    // modifiers
    ecs::Scene &addScene(const std::string &id, RectangleOffset viewport,
                         Color backgroundColor = {255, 255, 255, 255}, bool isActive = true);

    void run();
    void processEvents();
    void update();
    void render();

private:
    Fey(Specification spec, unique_ptr_guard_sdl &&guardSDL, unique_ptr_guard_bgfx &&guardBGFX,
        bool inited = false);
    bool m_inited;

    Specification m_specification;

    bool m_running = false;
    Clock m_clock;

    unique_ptr_guard_sdl m_guardSDL;
    unique_ptr_guard_bgfx m_guardBGFX;
    AssetManager m_assetManager;

    EventManager m_eventManager{};

    ecs::RenderSystem m_renderSystem{m_eventManager};
    std::list<ecs::Scene> m_scenes{};
};
} // namespace feyerverx