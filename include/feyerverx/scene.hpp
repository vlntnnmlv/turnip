// Copyright 2025 Valentin Namleev

#pragma once

#include <vector>

#include <SDL3/SDL.h>

#include "feyerverx/assetManager.hpp"
#include "feyerverx/ecs/registry.hpp"
#include "feyerverx/ecs/system.hpp"
#include "feyerverx/renderer.hpp"

namespace feyerverx {
class Scene {
public:
    void init();
    void update();
    void processEvent(const SDL_Event &event);

private:
    AssetManager m_assetManager{};
    Renderer m_renderer{};

    ecs::Registry m_registry{};
    std::vector<ecs::ISystem> m_systems{};
};
} // namespace feyerverx
