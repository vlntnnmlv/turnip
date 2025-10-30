// Copyright 2025 Valentin Namleev

#pragma once

#include <list>

#include "feyerverx/renderer.hpp"

#include "feyerverx/ecs/components/cameraComponent.hpp"
#include "feyerverx/ecs/scene.hpp"
#include "feyerverx/ecs/system.hpp"

#include "feyerverx/events/event.hpp"

namespace feyerverx::ecs {
struct CameraComponent;
class RenderSystem final : public ISystem {
public:
    explicit RenderSystem(EventManager &eventManager);
    ~RenderSystem() override { std::println("~RenderSystem"); }

    RenderSystem(const RenderSystem &) = delete;
    void operator=(const RenderSystem &) = delete;
    RenderSystem(RenderSystem &&other) noexcept
        : ISystem(std::move(other)), m_renderer(std::move(other.m_renderer)),
          m_cameraEntities(std::move(other.m_cameraEntities)) {}

    void operator=(const RenderSystem &&) = delete;

    void update(float deltaTime, const std::shared_ptr<Registry> &registry) override;
    void render(const Scene &scene, uint16_t viewID);

    bool onWindowResized(const WindowResizedEvent &event);

private:
    std::unique_ptr<Renderer> m_renderer;
    std::set<Entity> m_cameraEntities{};

    void setView(CameraComponent *cameraComponent, uint16_t viewID);

    void updateVectors(CameraComponent *cameraComponent);
};
} // namespace feyerverx