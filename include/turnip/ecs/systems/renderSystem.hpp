// Copyright 2025 Valentin Namleev

#pragma once

#include <iterator>
#include <memory>
#include <raylib-cpp.hpp>
#include <string>

#include "./turnip/ecs/components/colorComponent.hpp"
#include "./turnip/ecs/components/graphComponent.hpp"
#include "./turnip/ecs/components/renderTransformComponent.hpp"
#include "./turnip/ecs/components/spriteComponent.hpp"
#include "./turnip/ecs/components/textComponent.hpp"
#include "./turnip/ecs/components/transformComponent.hpp"
#include "./turnip/ecs/registry.hpp"
#include "./turnip/ecs/system.hpp"
#include "./turnip/rectangleUtils.hpp"

namespace turnip::ecs {
class RenderSystem : protected ISystem {
public:
    RenderSystem(Registry &_Registry, std::unique_ptr<raylib::Window> &_Window);

    void Update(float _DeltaTime) override;
    void SetBackgroundColor(Color _Color);

private:
    void Render();
    void RenderSprites();
    void RenderTexts();
    void RenderGraphs();
    void RenderDebug();

    Rectangle GetRenderRect(EntityID _EntityID, TransformComponent *_TransformComponent);

    Color m_BackgroundColor;
    std::unique_ptr<raylib::Window> &m_Window;
};
} // namespace turnip::ecs