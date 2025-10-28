// Copyright 2025 Valentin Namleev

#include "feyerverx/events/eventManager.hpp"

#include "feyerverx/ecs/components/cameraComponent.hpp"
#include "feyerverx/ecs/scene.hpp"

#include <print>

namespace feyerverx::ecs {
Scene::Scene(const std::string &id, EventManager &eventManager, const RectangleOffset viewport,
             const Color backgroundColor) noexcept
    : IIdentifiable(id), m_eventManager(eventManager), m_registry{std::make_shared<Registry>()},
      m_backgroundColor{backgroundColor} {

    auto cameraEntity = m_registry->createEntity();
    cameraEntity.addComponent<CameraComponent>(CameraType::ORTHOGRAPHIC, viewport, 0, 1000);

    auto cameraComponent = cameraEntity.getComponent<CameraComponent>();

    m_eventManager.subscribe<WindowResizedEvent>(
        [cameraComponent](const WindowResizedEvent &event) -> void {
            std::println("Window resized {} {}", event.windowSize.x, event.windowSize.y);
            cameraComponent->updateViewport(cameraComponent->viewportScaled, event.windowSize);
        });
}

Entity Scene::addEntity() const { return m_registry->createEntity(); }

void Scene::update(const float deltaTime) const {
    for (const std::unique_ptr<ISystem> &system : m_systems) {
        system->update(deltaTime, m_registry);
    }
}
} // namespace feyerverx
