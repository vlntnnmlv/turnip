// Copyright 2025 Valentin Namleev

#pragma once

#include "./turnip/ecs/registry.hpp"

namespace turnip::ecs {
struct ISystem {
    ISystem(Registry &_Registry);
    virtual ~ISystem() = default;
    virtual void Update(float _DeltaTime) = 0;

protected:
    Registry &m_Registry;
};
} // namespace turnip::ecs
