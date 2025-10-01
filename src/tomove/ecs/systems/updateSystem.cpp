// Copyright 2025 Valentin Namleev

#include "./turnip/ecs/systems/updateSystem.hpp"

namespace feyerverx::ecs {
UpdateSystem::UpdateSystem(Registry &_Registry) : ISystem(_Registry) {}

void UpdateSystem::Update(float _DeltaTime) {
    m_Time += _DeltaTime;
    for (const auto &e : m_Registry.With<UpdateComponent>()) {
        m_Registry.GetComponent<UpdateComponent>(e)->onUpdate(m_Time, _DeltaTime);
    }
}
} // namespace feyerverx::ecs
