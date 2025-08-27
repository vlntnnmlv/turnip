// Copyright 2025 Valentin Namleev

#pragma once

#include "../../rectangleUtils.hpp"
#include "../components/colorComponent.hpp"
#include "../components/graphComponent.hpp"
#include "../components/renderTransformComponent.hpp"
#include "../components/spriteComponent.hpp"
#include "../components/textComponent.hpp"
#include "../components/transformComponent.hpp"
#include "../registry.hpp"
#include "../system.hpp"

#include <iterator>
#include <memory>
#include <raylib-cpp.hpp>
#include <string>

// TODO: Add layering system (order of rendering)

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