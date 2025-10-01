// Copyright 2025 Valentin Namleev

#include "feyerverx/ecs/system.hpp"

namespace feyerverx::ecs {
ISystem::ISystem(Registry &_Registry) : m_Registry(_Registry) {}
} // namespace feyerverx::ecs
