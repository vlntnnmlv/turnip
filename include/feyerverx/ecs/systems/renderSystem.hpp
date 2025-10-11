// Copyright 2025 Valentin Namleev

#pragma once

#include "feyerverx/ecs/system.hpp"
#include "feyerverx/renderer.hpp"

namespace feyerverx::ecs {
class RenderSystem final : public ISystem {

public:
    RenderSystem();
    ~RenderSystem() override = default;

    void update(float deltaTime) override;
    void enqueueScene(Scene &scene) override;

private:
    Renderer m_renderer;
};
} // namespace feyerverx