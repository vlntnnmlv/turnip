// Copyright 2025 Valentin Namleev

#pragma once

#include <chrono>
#include <memory>
#include <string>

#include "feyerverx/ecs/scene.hpp"
#include "feyerverx/ecs/systems/renderSystem.hpp"

#include "feyerverx/camera.hpp"
#include "feyerverx/clock.hpp"
#include "feyerverx/guardBGFX.hpp"
#include "feyerverx/guardSDL.hpp"

namespace feyerverx {
class Fey {
public:
    Fey(std::string title, float width, float height);
    ~Fey() = default;

    AssetManager &assetManager();

    ecs::Scene &addScene(const std::string &id, bool isActive = false);

    void run();
    void processEvents();
    void update();

private:
    void initCamera();

    std::string m_title;
    float m_width;
    float m_height;

    Clock m_clock;

    bool m_running = false;

    GuardSDL m_guardSDL;
    GuardBGFX m_guardBGFX;
    AssetManager m_assetManager;

    std::unique_ptr<ICamera> m_camera;

    std::vector<ecs::Scene> m_scenes{};

    std::vector<std::unique_ptr<ecs::ISystem>> m_globalSystems{};
};
} // namespace feyerverx