// Copyright 2025 Valentin Namleev

#include "feyerverx/ecs/scene.hpp"

namespace feyerverx::ecs {
size_t Scene::nextUID = 0;

Scene::Scene(const std::string &id) noexcept : ID(id), uid(nextUID++) {}

Entity Scene::addEntity() { return m_registry.createEntity(); }
void Scene::addSystem(std::unique_ptr<ISystem> &&system) { m_systems.push_back(std::move(system)); }

Registry &Scene::registry() { return m_registry; }
uiBuilder &Scene::builder() { return m_builder; }

void Scene::update(float deltaTime) {
    for (std::unique_ptr<ISystem> const &system : m_systems)
        system->enqueueScene(*this);

    for (std::unique_ptr<ISystem> const &system : m_systems)
        system->update(deltaTime);
}
void Scene::enqueueEvent(const SDL_Event &_event) {}
} // namespace feyerverx
