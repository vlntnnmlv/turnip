// Copyright 2025 Valentin Namleev

#include "feyerverx/ecs/system.hpp"

namespace feyerverx::ecs {
ISystem::ISystem(const std::string &id) : IIdentifiable(id) {}
} // namespace feyerverx