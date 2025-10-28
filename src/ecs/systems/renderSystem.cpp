// Copyright 2025 Valentin Namleev

#include <ranges>

#include "feyerverx/ecs/components/spriteComponent.hpp"
#include "feyerverx/ecs/components/transformComponent.hpp"
#include "feyerverx/ecs/scene.hpp"
#include "feyerverx/ecs/systems/renderSystem.hpp"

#include "bx/math.h"
#include "feyerverx/ecs/components/cameraComponent.hpp"

namespace feyerverx::ecs {

RenderSystem::RenderSystem(EventManager &eventManager)
    : ISystem("render_system", eventManager), m_renderer(std::move(Renderer::create())) {}

void RenderSystem::update(float deltaTime, const std::shared_ptr<Registry> &registry) { /*TODO:*/ }

void RenderSystem::render(const Scene &scene, const uint16_t viewID) {
    auto registry = scene.registry();

    const auto toRender = registry->with<TransformComponent, SpriteComponent>() |
                          std::views::transform([&registry](const EntityID entityID) {
                              return Entity(entityID, registry.get());
                          }) |
                          std::ranges::to<std::vector>();

    if (const auto camera = getSceneCameraComponent(scene))
        camera->setView(viewID);

    bgfx::setViewClear(viewID, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, scene.backgroundColor());

    for (const Entity &entity : toRender) {
        m_renderer->renderTexture(entity.getComponent<SpriteComponent>()->texture,
                                  entity.getComponent<TransformComponent>()->rect, viewID);
    }
}
} // namespace feyerverx::ecs