// Copyright 2025 Valentin Namleev

#include <ranges>

#include "feyerverx/ecs/components/spriteComponent.hpp"
#include "feyerverx/ecs/components/transformComponent.hpp"
#include "feyerverx/ecs/systems/renderSystem.hpp"

namespace feyerverx::ecs {

RenderSystem::RenderSystem() : m_renderer(std::move(Renderer::create())) {}

void RenderSystem::enqueueScene(Scene &scene) {
    m_entityQueue.clear();
    Registry &registry = scene.registry();
    m_entityQueue = scene.registry().with<ecs::TransformComponent, ecs::SpriteComponent>() |
                    std::views::transform(
                        [&registry](EntityID entityID) { return Entity(entityID, &(registry)); }) |
                    std::ranges::to<std::vector>();
}

void RenderSystem::update(float deltaTime) {
    for (Entity &entity : m_entityQueue) {
        m_renderer.renderTexture(entity.getComponent<SpriteComponent>()->texture,
                                 entity.getComponent<TransformComponent>()->rect);
    }
}
} // namespace feyerverx::ecs