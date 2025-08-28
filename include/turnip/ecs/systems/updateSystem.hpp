// Copyright 2025 Valentin Namleev

#pragma once

#include "../components/updateComponent.hpp"
#include "../registry.hpp"
#include "../system.hpp"

namespace turnip::ecs {
class UpdateSystem : protected ISystem {
public:
    UpdateSystem(Registry &_Registry);

    void Update(float _DeltaTime) override;
};
} // namespace turnip::ecs
