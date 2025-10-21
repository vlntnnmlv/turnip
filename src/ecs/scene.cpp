// Copyright 2025 Valentin Namleev

#include "feyerverx/ecs/scene.hpp"

#include "feyerverx/camera.hpp"

namespace feyerverx::ecs {
Scene::Scene(const std::string &id, const RectangleOffset viewport,
             const Color backgroundColor) noexcept
    : IIdentifiable(id), m_registry{std::make_shared<Registry>()},
      m_camera{
          std::move(CameraPerspective::create(viewport, {400, 300, 200}, 120, 0.1f))},
      m_backgroundColor{backgroundColor} {}

Entity Scene::addEntity() { return m_registry->createEntity(); }
void Scene::addSystem(std::unique_ptr<ISystem> &&system) { m_systems.push_back(std::move(system)); }

std::shared_ptr<Registry> Scene::registry() { return m_registry; }
UIBuilder &Scene::builder() { return m_builder; }
std::unique_ptr<ICamera> &Scene::camera() { return m_camera; }
} // namespace feyerverx
