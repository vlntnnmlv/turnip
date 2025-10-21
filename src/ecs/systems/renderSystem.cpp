// Copyright 2025 Valentin Namleev

#include <ranges>

#include "feyerverx/ecs/components/spriteComponent.hpp"
#include "feyerverx/ecs/components/transformComponent.hpp"
#include "feyerverx/ecs/scene.hpp"
#include "feyerverx/ecs/systems/renderSystem.hpp"

namespace feyerverx::ecs {

RenderSystem::RenderSystem()
    : ISystem("render_system"), m_renderer(std::move(Renderer::create())) {}

void RenderSystem::update(float deltaTime, std::shared_ptr<Registry> registry) { /*TODO:*/ }

void RenderSystem::render(Scene &scene, const uint16_t viewID) {
    auto registry = scene.registry();

    const auto toRender = registry->with<TransformComponent, SpriteComponent>() |
                          std::views::transform([&registry](const EntityID entityID) {
                              return Entity(entityID, registry.get());
                          }) |
                          std::ranges::to<std::vector>();

    scene.camera()->setView(viewID);

    bgfx::setViewClear(viewID, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x45454545 + viewID);

    for (const Entity &entity : toRender) {
        m_renderer->renderTexture(entity.getComponent<SpriteComponent>()->texture,
                                  entity.getComponent<TransformComponent>()->rect, viewID);
    }
}
} // namespace feyerverx::ecs