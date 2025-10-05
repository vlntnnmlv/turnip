// Copyright 2025 Valentin Namleev

#include "feyerverx/ecs/scene.hpp"

namespace feyerverx::ecs {
size_t Scene::nextuid = 0;

Scene::Scene() noexcept : uid(nextuid++) {}

ecs::Entity Scene::addEntity() { return m_registry.CreateEntity(); }
ecs::Registry &Scene::registry() { return m_registry; }

void Scene::update(float deltaTime) {
    for (std::unique_ptr<ecs::ISystem> &system : m_systems)
        system->update(deltaTime);
}
void Scene::enqueueEvent(const SDL_Event &_event) {}
} // namespace feyerverxs
