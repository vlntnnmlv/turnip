// Copyright 2025 Valentin Namleev

#pragma once

// #include <functional>
// #include <iterator>
// #include <memory>
// #include <string>

// #include "feyerverx/ecs/components/colorComponent.hpp"
// #include "feyerverx/ecs/components/renderTransformComponent.hpp"
// #include "feyerverx/ecs/components/spriteComponent.hpp"
// #include "feyerverx/ecs/components/textComponent.hpp"
// #include "feyerverx/ecs/components/transformComponent.hpp"
// #include "feyerverx/ecs/registry.hpp"
#include "feyerverx/ecs/scene.hpp"
#include "feyerverx/renderer.hpp"

#include "feyerverx/ecs/system.hpp"

namespace feyerverx::ecs {
class RenderSystem final : public ISystem {
public:
    RenderSystem();
    ~RenderSystem() override = default;

    void update(float deltaTime) override;
    void enqueueScene(Scene &scene) override;

private:
    Renderer m_renderer{};
};
} // namespace feyerverx
// using RenderCallback = std::function<void(std::vector<EntityID> &, ecs::Registry &)>;
// using ComponentRenderers = std::vector<std::pair<ComponentTypeSet, RenderCallback>>;

// class RenderSystem : protected ISystem {
// public:
//     void update(float _DeltaTime) override;
//     void SetBackgroundColor(Color _Color);
//     void RegisterRenderer(const ComponentTypeSet &_ComponentTypeSet,
//                           RenderCallback _RenderCallback);

//     static Rectangle GetRenderRect(TransformComponent *_TransformComponent,
//                                    RenderTransformComponent *_RenderTransformComponent);

//     void ShowFPS(bool _Value);

// private:
//     ComponentRenderers m_ComponentRenderers{};

//     void Render();
//     void RenderSprites(std::vector<EntityID> &_ToRender);
//     void RenderTexts(std::vector<EntityID> &_ToRender);
//     void RenderDebug(std::vector<EntityID> &_ToRender);

//     Color m_BackgroundColor;
//     bool m_ShowFPS;
// };
// } // namespace feyerverx::ecs