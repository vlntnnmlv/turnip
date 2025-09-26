// Copyright 2025 Valentin Namleev

#include "turnip/ecs/entity.hpp"
#include "turnip/ecs/registry.hpp"

namespace turnip::ecs {
Entity::Entity() : m_ID(NullEntityID) {}
Entity::Entity(EntityID _ID, Registry *const _Registry) : m_ID(_ID), m_Registry(_Registry) {}
EntityID Entity::ID() const { return m_ID; }
} // namespace turnip::ecs