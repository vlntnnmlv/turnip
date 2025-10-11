// Copyright 2025 Valentin Namleev

#pragma once

#include <memory>
#include <string>

#include "feyerverx/ecs/scene.hpp"
#include "feyerverx/ecs/systems/renderSystem.hpp"

#include "feyerverx/camera.hpp"
#include "feyerverx/clock.hpp"
#include "feyerverx/guardBGFX.hpp"
#include "feyerverx/guardSDL.hpp"

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

class Fey {
public:
    static std::expected<Fey, Error> create(const std::string &title, float width, float height);

    Fey(const Fey &fey) = delete;
    Fey &operator=(const Fey &fey) = delete;
    Fey(Fey &&other) noexcept;
    Fey &operator=(Fey &&other) = delete;

    ~Fey() = default;

    AssetManager &assetManager();

    ecs::Scene &addScene(const std::string &id, bool isActive = false);
    void initGlobalSystems();

    void run();
    void processEvents();
    void update();

private:
    Fey(std::string title, float width, float height, unique_ptr_guard_sdl &&guardSDL,
        unique_ptr_guard_bgfx &&guardBGFX, std::unique_ptr<ICamera> &&camera);

    bool m_running = false;
    Clock m_clock;

    std::string m_title;
    float m_width;
    float m_height;

    unique_ptr_guard_sdl m_guardSDL;
    unique_ptr_guard_bgfx m_guardBGFX;
    std::unique_ptr<ICamera> m_camera; // TODO: move to scene class
    AssetManager m_assetManager;
    std::vector<ecs::Scene> m_scenes{};
    std::vector<std::unique_ptr<ecs::ISystem>> m_globalSystems{};
};
} // namespace feyerverx