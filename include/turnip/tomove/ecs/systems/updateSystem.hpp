// Copyright 2025 Valentin Namleev

#pragma once

#include "./turnip/ecs/components/updateComponent.hpp"
#include "./turnip/ecs/registry.hpp"
#include "./turnip/ecs/system.hpp"

namespace turnip::ecs {
class UpdateSystem : protected ISystem {
public:
    UpdateSystem(Registry &_Registry);

    void Update(float _DeltaTime) override;

private:
    float m_Time{0.0f};
};
} // namespace turnip::ecs
