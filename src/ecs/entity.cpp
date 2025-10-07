// Copyright 2025 Valentin Namleev

#include "feyerverx/ecs/entity.hpp"
#include "feyerverx/ecs/registry.hpp"

namespace feyerverx::ecs {
Entity::Entity() : m_ID(NullEntityID) {}
Entity::Entity(EntityID _ID, Registry *const registry) : m_ID(_ID), m_Registry(registry) {}
EntityID Entity::ID() const { return m_ID; }
} // namespace feyerverx::ecs