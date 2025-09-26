// Copyright 2025 Valentin Namleev

#include "turnip/ecs/system.hpp"

namespace turnip::ecs {
ISystem::ISystem(Registry &_Registry) : m_Registry(_Registry) {}
} // namespace turnip::ecs
