// Copyright 2025 Valentin Namleev

#include "./turnip/ecs/entity.hpp"

namespace turnip::ecs {
Entity::Entity(Registry &_Registry) : m_Registry(_Registry) { m_ID = m_Registry.CreateEntity(); }
EntityID Entity::ID() const { return m_ID; }
} // namespace turnip::ecs