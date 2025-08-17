// Copyright 2025 Valentin Namleev

#pragma once

#include "../components/updateComponent.hpp"
#include "../registry.hpp"
#include "../system.hpp"

namespace turnip::ecs {
class UpdateSystem : protected ISystem {
public:
    UpdateSystem(Registry &_Registry) : ISystem(_Registry) {}

    void Update(float _DeltaTime) override {
        for (const auto &e : m_Registry.With<UpdateComponent>()) {
            m_Registry.GetComponent<UpdateComponent>(e)->onUpdate(_DeltaTime);
        }
    };
};
} // namespace turnip::ecs
