// Copyright 2025 Valentin Namleev

#include "./ecs/systems/updateSystem.hpp"

namespace turnip::ecs {
UpdateSystem::UpdateSystem(Registry &_Registry) : ISystem(_Registry) {}

void UpdateSystem::Update(float _DeltaTime) {
    for (const auto &e : m_Registry.With<UpdateComponent>()) {
        m_Registry.GetComponent<UpdateComponent>(e)->onUpdate(_DeltaTime);
    }
}
} // namespace turnip::ecs
