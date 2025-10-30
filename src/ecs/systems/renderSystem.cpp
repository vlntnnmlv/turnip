// Copyright 2025 Valentin Namleev

#include <ranges>

#include "feyerverx/ecs/components/cameraComponent.hpp"
#include "feyerverx/ecs/components/spriteComponent.hpp"
#include "feyerverx/ecs/components/transformComponent.hpp"
#include "feyerverx/ecs/scene.hpp"
#include "feyerverx/ecs/systems/renderSystem.hpp"

namespace feyerverx::ecs {

RenderSystem::RenderSystem(EventManager &eventManager)
    : ISystem("render_system", eventManager), m_renderer(std::move(Renderer::create())) {
    m_eventManager.subscribe<WindowResizedEvent>(
        [this](const WindowResizedEvent &event) { return onWindowResized(event); });
}

void RenderSystem::update(float deltaTime, const std::shared_ptr<Registry> &registry) {}

void RenderSystem::render(const Scene &scene, const uint16_t viewID) {
    auto registry = scene.registry();

    const auto toRender = registry->with<TransformComponent, SpriteComponent>() |
                          std::views::transform([&registry](const EntityID entityID) {
                              return Entity(entityID, registry.get());
                          }) |
                          std::ranges::to<std::vector>();

    if (const auto cameraEntityID = scene.registry()->with<CameraComponent>().front()) {
        const auto cameraComponent =
            scene.registry()->getComponent<CameraComponent>(cameraEntityID);

        m_cameraEntities.insert(Entity(cameraEntityID, scene.registry().get()));
        updateVectors(cameraComponent);
        setView(cameraComponent, viewID);
        bgfx::touch(viewID);
        bgfx::setViewClear(viewID, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, cameraComponent->color);
    }

    for (const Entity &entity : toRender) {
        m_renderer->renderTexture(entity.getComponent<SpriteComponent>()->texture,
                                  entity.getComponent<TransformComponent>()->rect, viewID);
    }

    m_renderer->renderLine({0, 0, 0}, {100, 0, 0}, Color{255, 0, 0, 255}, viewID);
    m_renderer->renderLine({0, 0, 0}, {0, 100, 0}, Color{255, 0, 0, 255}, viewID);
    m_renderer->renderLine({0, 0, 0}, {0, 0, 100}, Color{255, 0, 0, 255}, viewID);
}

void RenderSystem::updateVectors(CameraComponent *cameraComponent) {
    static bx::Vec3 forward{0.0f, 0.0f, -1.0f};
    static bx::Vec3 upWorld{0.0f, -1.0f, 0.0f};

    const bx::Vec3 eye{cameraComponent->position.x, cameraComponent->position.y,
                       cameraComponent->position.z};

    const bx::Quaternion rotation = bx::fromEuler(bx::Vec3{
        cameraComponent->rotation.x, cameraComponent->rotation.y, cameraComponent->rotation.z});

    const bx::Vec3 lookingAt = bx::add(eye, bx::mul(forward, rotation));
    cameraComponent->lookingAt = Vector3f{lookingAt.x, lookingAt.y, lookingAt.z};

    const bx::Vec3 up = bx::mul(upWorld, rotation);
    cameraComponent->up = Vector3f{up.x, up.y, up.z};
}

void RenderSystem::setView(CameraComponent *cameraComponent, const uint16_t viewID) {
    float projectionMatrix[16];
    switch (cameraComponent->type) {
    case CameraType::ORTHOGRAPHIC: {
        bx::mtxOrtho(projectionMatrix, cameraComponent->viewport.left,
                     cameraComponent->viewport.right, cameraComponent->viewport.bottom,
                     cameraComponent->viewport.top, cameraComponent->near, cameraComponent->far,
                     0.0f, bgfx::getCaps()->homogeneousDepth);
        break;
    }
    case CameraType::PERSPECTIVE:
        bx::mtxProj(projectionMatrix, cameraComponent->FOV, 1, cameraComponent->near,
                    cameraComponent->far, bgfx::getCaps()->homogeneousDepth);
        break;
    }

    float viewMatrix[16];
    const bx::Vec3 cameraPosition(cameraComponent->position.x, cameraComponent->position.y,
                                  cameraComponent->position.z);
    const bx::Vec3 cameraLookingAt(cameraComponent->lookingAt.x, cameraComponent->lookingAt.y,
                                   cameraComponent->lookingAt.z);
    const bx::Vec3 cameraUp(cameraComponent->up.x, cameraComponent->up.y, cameraComponent->up.z);
    bx::mtxLookAt(viewMatrix, cameraPosition, cameraLookingAt, cameraUp);

    bgfx::setViewTransform(viewID, viewMatrix, projectionMatrix);
    bgfx::setViewRect(viewID, static_cast<uint16_t>(cameraComponent->viewport.left),
                      static_cast<uint16_t>(cameraComponent->viewport.top),
                      static_cast<uint16_t>(cameraComponent->viewport.right),
                      static_cast<uint16_t>(cameraComponent->viewport.bottom));
}

bool RenderSystem::onWindowResized(const WindowResizedEvent &event) {
    if (m_cameraEntities.empty())
        return false;

    for (const Entity &entity : m_cameraEntities) {
        const auto cameraComponent = entity.getComponent<CameraComponent>();
        if (cameraComponent->type != CameraType::ORTHOGRAPHIC)
            continue;

        cameraComponent->updateViewport(event.windowSize);
    }

    return true;
}
} // namespace feyerverx::ecs