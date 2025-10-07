// Copyright 2025 Valentin Namleev

#pragma once

#include "feyerverx/ecs/registry.hpp"
#include "feyerverx/ecs/scene.hpp"

namespace feyerverx::ecs {
class Scene;

struct ISystem {
    ISystem() = default;
    virtual ~ISystem() = default;
    virtual void update(float deltaTime) = 0;
    virtual void enqueueScene(Scene &scene) = 0;
    virtual void processEvent(const SDL_Event &event) {}

protected:
    std::vector<Entity> m_entityQueue{};
};
} // namespace feyerverx::ecs
