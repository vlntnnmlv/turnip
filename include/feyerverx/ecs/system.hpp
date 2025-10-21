// Copyright 2025 Valentin Namleev

#pragma once

#include <SDL3/SDL_events.h>

#include "feyerverx/ecs/entity.hpp"
#include "feyerverx/identifiable.hpp"

namespace feyerverx::ecs {
class Scene;

struct ISystem : IIdentifiable {
    explicit ISystem(const std::string &id);
    virtual ~ISystem() = default;
    virtual void update(float deltaTime, std::shared_ptr<Registry> registry) = 0;

protected:
    std::vector<Entity> m_entityQueue{};
};
} // namespace feyerverx::ecs
