// Copyright 2025 Valentin Namleev

#include "feyerverx/ecs/scene.hpp"

#include "feyerverx/ecs/components/cameraComponent.hpp"
#include "feyerverx/logger.hpp"

namespace feyerverx::ecs {
Scene::Scene(const std::string &id, const RectangleOffset viewport,
             const Color backgroundColor) noexcept
    : IIdentifiable(id), m_registry{std::make_shared<Registry>()},
      m_backgroundColor{backgroundColor} {}

Entity Scene::addEntity() const { return m_registry->createEntity(); }

void Scene::setOrthogonalCamera(const Vector2f windowSize, const RectangleOffset viewport,
                                const float near, const float far, Vector3f position,
                                Vector3f lookingAt, Vector3f up, Color color) const {
    auto cameraEntity = m_registry->createEntity();
    cameraEntity.addComponent<CameraComponent>(viewport, near, far, position, lookingAt, up, color);

    auto *cameraComponent = cameraEntity.getComponent<CameraComponent>();
    cameraComponent->rotation.x = 35;
    cameraComponent->rotation.y = -45;

    cameraComponent->updateViewport(windowSize);
    Logger::instance().log(LogLevel::Info, "Set orthogonal camera");
}

void Scene::setPerspectiveCamera(const float FOV, const float near, const float far,
                                 Vector3f position, Vector3f lookingAt, Vector3f up,
                                 Color color) const {
    auto cameraEntity = m_registry->createEntity();
    cameraEntity.addComponent<CameraComponent>(FOV, near, far, position, lookingAt, up, color);
    Logger::instance().log(LogLevel::Info, "Set perspective camera");
}

void Scene::update(const float deltaTime) const {
    for (const std::unique_ptr<ISystem> &system : m_systems) {
        system->update(deltaTime, m_registry);
    }
}
} // namespace feyerverx
