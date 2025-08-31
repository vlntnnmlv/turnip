// Copyright 2025 Valentin Namleev

#pragma once

#include <iterator>
#include <memory>
#include <raylib-cpp.hpp>
#include <string>

#include "./turnip/ecs/components/colorComponent.hpp"
#include "./turnip/ecs/components/renderTransformComponent.hpp"
#include "./turnip/ecs/components/spriteComponent.hpp"
#include "./turnip/ecs/components/textComponent.hpp"
#include "./turnip/ecs/components/transformComponent.hpp"
#include "./turnip/ecs/registry.hpp"
#include "./turnip/ecs/system.hpp"
#include "./turnip/rectangleUtils.hpp"

namespace turnip::ecs {

using RenderCallback = std::function<void(std::vector<EntityID> &, ecs::Registry &)>;
using ComponentRenderers = std::vector<std::pair<ComponentTypeSet, RenderCallback>>;

class RenderSystem : protected ISystem {
public:
    RenderSystem(Registry &_Registry, std::unique_ptr<raylib::Window> &_Window);

    void Update(float _DeltaTime) override;
    void SetBackgroundColor(Color _Color);
    void RegisterRenderer(const ComponentTypeSet &_ComponentTypeSet,
                          RenderCallback _RenderCallback);

    static Rectangle GetRenderRect(TransformComponent *_TransformComponent,
                                   RenderTransformComponent *_RenderTransformComponent);

private:
    ComponentRenderers m_ComponentRenderers{};

    void Render();
    void RenderSprites(std::vector<EntityID> &_ToRender);
    void RenderTexts(std::vector<EntityID> &_ToRender);
    void RenderDebug(std::vector<EntityID> &_ToRender);
    // void RenderGraphs();

    Color m_BackgroundColor;
    std::unique_ptr<raylib::Window> &m_Window;
};
} // namespace turnip::ecs