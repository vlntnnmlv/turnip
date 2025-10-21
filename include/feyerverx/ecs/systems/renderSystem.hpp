// Copyright 2025 Valentin Namleev

#pragma once

#include <list>

#include "feyerverx/ecs/system.hpp"
#include "feyerverx/renderer.hpp"

namespace feyerverx::ecs {
class RenderSystem final : public ISystem {

public:
    RenderSystem();
    ~RenderSystem() override = default;

    RenderSystem(const RenderSystem &) = delete;
    void operator=(const RenderSystem &) = delete;
    RenderSystem(RenderSystem &&) = default;
    void operator=(const RenderSystem &&) = delete;

    void render(Scene &scene, uint16_t viewID);
    void update(float deltaTime, std::shared_ptr<Registry> registry) override;

private:
    std::unique_ptr<Renderer> m_renderer;
    std::list<std::pair<Scene &, std::vector<Entity>>> m_scenesEntities{};
};
} // namespace feyerverx