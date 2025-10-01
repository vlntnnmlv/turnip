// Copyright 2025 Valentin Namleev

#pragma once

#include "feyerverx/ecs/registry.hpp"

namespace feyerverx::ecs {
struct ISystem {
    ISystem(Registry &_Registry);
    virtual ~ISystem() = default;
    virtual void Update(float _DeltaTime) = 0;

protected:
    Registry &m_Registry;
};
} // namespace feyerverx::ecs
