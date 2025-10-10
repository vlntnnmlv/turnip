// Copyright 2025 Valentin Namleev

#pragma once

#include <memory>
#include <string>

#include "feyerverx/ecs/scene.hpp"
#include "feyerverx/ecs/systems/renderSystem.hpp"

#include "feyerverx/camera.hpp"
#include "feyerverx/clock.hpp"
#include "feyerverx/error.hpp"
#include "feyerverx/guardBGFX.hpp"
#include "feyerverx/guardSDL.hpp"

namespace feyerverx {
class Fey {
public:
    static std::variant<Fey, Error> create(const std::string &title, float width, float height);

    Fey(Fey &&other) noexcept;
    ~Fey() = default;

    AssetManager &assetManager();

    ecs::Scene &addScene(const std::string &id, bool isActive = false);

    void run();
    void processEvents();
    void update();

private:
    Fey(std::string title, float width, float height, std::unique_ptr<GuardSDL> &guardSDL,
        std::unique_ptr<GuardBGFX> &guardBGFX);

    void initCamera();

    std::string m_title;
    float m_width;
    float m_height;

    Clock m_clock;

    bool m_running = false;

    AssetManager m_assetManager;

    std::unique_ptr<GuardSDL> m_guardSDL;
    std::unique_ptr<GuardBGFX> m_guardBGFX;

    std::unique_ptr<ICamera> m_camera; // TODO: move to scene class

    std::vector<ecs::Scene> m_scenes{};

    std::vector<std::unique_ptr<ecs::ISystem>> m_globalSystems{};
};
} // namespace feyerverx