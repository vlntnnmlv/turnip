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
    Fey(const std::string &title, int _width, int _height);
    ~Fey();

    AssetManager &assetManager();

    ecs::Scene &addScene(const std::string &id, bool isActive = false);

    void run();
    void processEvents();
    void update();

private:
    void initCamera();

    std::string m_title;
    int m_width;
    int m_height;

    Clock m_clock;

    bool m_running;

    GuardSDL m_guardSDL;
    GuardBGFX m_guardBGFX;
    AssetManager m_assetManager;

    std::unique_ptr<ICamera> m_camera;

    std::vector<ecs::Scene> m_scenes{};

    std::vector<std::unique_ptr<ecs::ISystem>> m_globalSystems{};
};
} // namespace feyerverx