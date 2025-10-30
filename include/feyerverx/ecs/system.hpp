// Copyright 2025 Valentin Namleev

#pragma once

#include "feyerverx/identifiable.hpp"

#include "feyerverx/events/eventManager.hpp"

#include "feyerverx/ecs/entity.hpp"

namespace feyerverx::ecs {
class Scene;

struct ISystem : IIdentifiable {
    explicit ISystem(const std::string &id, EventManager &eventManager);
    ISystem(ISystem &&other) : m_eventManager(other.m_eventManager) {};
    virtual ~ISystem() = default;
    virtual void update(float deltaTime, const std::shared_ptr<Registry> &registry) = 0;

protected:
    EventManager &m_eventManager;
};
} // namespace feyerverx::ecs
