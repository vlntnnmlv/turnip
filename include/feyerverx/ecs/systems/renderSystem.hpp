// Copyright 2025 Valentin Namleev

#pragma once

#include <list>

#include "feyerverx/renderer.hpp"

#include "feyerverx/ecs/components/cameraComponent.hpp"
#include "feyerverx/ecs/scene.hpp"
#include "feyerverx/ecs/system.hpp"

namespace feyerverx::ecs {
struct CameraComponent;
class RenderSystem final : public ISystem {
public:
    explicit RenderSystem(EventManager &eventManager);
    ~RenderSystem() override = default;

    RenderSystem(const RenderSystem &) = delete;
    void operator=(const RenderSystem &) = delete;
    RenderSystem(RenderSystem &&) = default;
    void operator=(const RenderSystem &&) = delete;

    void render(const Scene &scene, uint16_t viewID);
    void update(float deltaTime, const std::shared_ptr<Registry> &registry) override;

private:
    std::unique_ptr<Renderer> m_renderer;
    std::list<std::pair<Scene &, std::vector<Entity>>> m_scenesEntities{};
    std::unordered_map<size_t, CameraComponent *> m_scenesCameras{};

    CameraComponent *getSceneCameraComponent(const Scene &scene) {
        if (!m_scenesCameras.contains(scene.UID())) {
            const EntityID cameraEntity = scene.registry()->with<CameraComponent>().front();
            m_scenesCameras[scene.UID()] =
                scene.registry()->getComponent<CameraComponent>(cameraEntity);
        }

        return m_scenesCameras[scene.UID()];
    }
};
} // namespace feyerverx