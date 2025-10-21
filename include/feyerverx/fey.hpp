// Copyright 2025 Valentin Namleev

#pragma once

#include <list>
#include <memory>
#include <string>

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
    static std::expected<Fey, Error> create(const std::string &name, float width, float height);
    static std::expected<Fey, Error> create(const Specification &specification);

    Fey(const Fey &fey) = delete;
    Fey &operator=(const Fey &fey) = delete;
    Fey(Fey &&other) noexcept;
    Fey &operator=(Fey &&other) = delete;

    ~Fey() = default;

    // getters
    Specification &specification();
    AssetManager &assetManager();

    // modifiers
    ecs::Scene &addScene(const std::string &id, RectangleOffset viewport,
                         Color backgroundColor = {255, 255, 255, 255}, bool isActive = true);

    template <typename T, typename... Args> void addSystem(Args &&...args) {
        static_assert(std::is_base_of_v<ecs::ISystem, T>, "T must derive from ISystem");
        m_systems.push_back(std::make_unique<T>(std::forward<Args>(args)...));
    }

    void run();
    void processEvents();
    void update();

private:
    Fey(Specification spec, unique_ptr_guard_sdl &&guardSDL, unique_ptr_guard_bgfx &&guardBGFX);

    Specification m_specification;

    bool m_running = false;
    Clock m_clock;

    unique_ptr_guard_sdl m_guardSDL;
    unique_ptr_guard_bgfx m_guardBGFX;
    AssetManager m_assetManager;

    ecs::RenderSystem m_renderSystem{};
    std::list<unique_ptr_system> m_systems{};
    std::list<ecs::Scene> m_scenes{};
};
} // namespace feyerverx