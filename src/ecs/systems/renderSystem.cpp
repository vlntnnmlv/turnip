// Copyright 2025 Valentin Namleev

#include <ranges>

#include "feyerverx/ecs/components/spriteComponent.hpp"
#include "feyerverx/ecs/components/transformComponent.hpp"
#include "feyerverx/ecs/systems/renderSystem.hpp"
#include "feyerverx/logger.hpp"

namespace feyerverx::ecs {
RenderSystem::RenderSystem() { m_renderer.init(); }

void RenderSystem::enqueueScene(Scene &scene) {
    m_entityQueue.clear();
    Registry &registry = scene.registry();
    m_entityQueue = scene.registry().With<ecs::TransformComponent, ecs::SpriteComponent>() |
                    std::views::transform(
                        [&registry](EntityID entityID) { return Entity(entityID, &(registry)); }) |
                    std::ranges::to<std::vector>();
}

void RenderSystem::update(float deltaTime) {
    for (Entity &entity : m_entityQueue) {
        m_renderer.renderTexture(entity.GetComponent<SpriteComponent>()->texture,
                                 entity.GetComponent<TransformComponent>()->rect);
    }
}
}
// RenderSystem::RenderSystem() {
//     RegisterRenderer(
//         {typeid(ecs::TransformComponent), typeid(ecs::SpriteComponent)},
//         [this](std::vector<EntityID> &_ToRender, [[maybe_unused]] ecs::Registry &_Registry) {
//             RenderSprites(_ToRender);
//         });

//     RegisterRenderer({typeid(ecs::TransformComponent), typeid(ecs::TextComponent)},
//                      [this](std::vector<EntityID> &_ToRender,
//                             [[maybe_unused]] ecs::Registry &_Registry) { RenderTexts(_ToRender);
//                             });

// #if DEBUG
//     RegisterRenderer({typeid(ecs::TransformComponent)},
//                      [this](std::vector<EntityID> &_ToRender,
//                             [[maybe_unused]] ecs::Registry &_Registry) { RenderDebug(_ToRender);
//                             });
// #endif
// }

// void RenderSystem::update([[maybe_unused]] float _DeltaTime) { Render(); }

// void RenderSystem::SetBackgroundColor(Color _Color) { m_BackgroundColor = _Color; }

// void RenderSystem::RegisterRenderer(const ComponentTypeSet &_ComponentTypeSet,
//                                     RenderCallback _RenderCallback) {
//     m_ComponentRenderers.emplace_back(_ComponentTypeSet, std::move(_RenderCallback));
// }

// void RenderSystem::Render() {
//     // BeginDrawing();

//     for (const auto &[componentTypeSet, renderCallback] : m_ComponentRenderers) {
//         std::vector<EntityID> toRender = m_registry.With(componentTypeSet);
//         if (toRender.empty())
//             continue;

//         renderCallback(toRender, m_registry);
//     }

//     // if (m_ShowFPS)
//     // DrawText(std::to_string(GetFPS()).c_str(), 0, 0, 24, RED);

//     // EndDrawing();
// }

// void RenderSystem::RenderSprites(std::vector<EntityID> &_ToRender) {
//     std::sort(_ToRender.begin(), _ToRender.end());

//     for (EntityID e : _ToRender) {
//         TransformComponent *transformComponent = m_registry.GetComponent<TransformComponent>(e);
//         RenderTransformComponent *renderTransformComponent =
//             m_registry.GetComponent<RenderTransformComponent>(e);
//         SpriteComponent *spriteComponent = m_registry.GetComponent<SpriteComponent>(e);
//         ColorComponent *colorComponent = m_registry.GetComponent<ColorComponent>(e);

//         Color color = colorComponent ? colorComponent->color : Color{255, 255, 255, 255};

//         // NPatchInfo patchInfo =
//         //     NPatchInfo{Rectangle{0, 0, static_cast<float>(spriteComponent->texture.width),
//         //                          static_cast<float>(spriteComponent->texture.height)},
//         //                static_cast<int>(spriteComponent->patch.left),
//         //                static_cast<int>(spriteComponent->patch.top),
//         //                static_cast<int>(spriteComponent->patch.right),
//         //                static_cast<int>(spriteComponent->patch.bottom),
//         //                NPATCH_NINE_PATCH};

//         Rectangle renderRect = GetRenderRect(transformComponent, renderTransformComponent);

//         // DrawTextureNPatch(spriteComponent->texture, patchInfo, renderRect, {0, 0}, 0, color);
//     }
// }

// void RenderSystem::RenderTexts(std::vector<EntityID> &_ToRender) {
//     for (EntityID e : _ToRender) {
//         TransformComponent *transformComponent = m_registry.GetComponent<TransformComponent>(e);
//         TextComponent *textComponent = m_registry.GetComponent<TextComponent>(e);
//         ColorComponent *colorComponent = m_registry.GetComponent<ColorComponent>(e);

//         Color color = colorComponent ? colorComponent->color : Color{255, 255, 255, 255};

//         // Vector2f textSize = textComponent->font.MeasureText(
//         //     textComponent->text.c_str(), textComponent->fontSize, textComponent->spacing);

//         RenderTransformComponent *renderTransformComponent =
//             m_registry.GetComponent<RenderTransformComponent>(e);

//         Rectangle renderRect = GetRenderRect(transformComponent, renderTransformComponent);
//         // Vector2f center = RectangleUtils::Center(renderRect);

//         // Vector2f textPosition = Vector2f{center.x - textSize.x * 0.5f, center.y - textSize.y *
//         // 0.5f};

//         // textComponent->font.DrawText(textComponent->text.c_str(), textPosition,
//         //                              textComponent->fontSize, textComponent->spacing, color);
//     }
// }

// void RenderSystem::RenderDebug(std::vector<EntityID> &_ToRender) {
//     for (EntityID e : _ToRender) {
//         TransformComponent *transformComponent = m_registry.GetComponent<TransformComponent>(e);
//         RenderTransformComponent *renderTransformComponent =
//             m_registry.GetComponent<RenderTransformComponent>(e);

//         // DrawRectangleLinesEx(GetRenderRect(transformComponent, renderTransformComponent), 1,
//         // RED);
//     }
// }

// Rectangle RenderSystem::GetRenderRect(TransformComponent *_TransformComponent,
//                                       RenderTransformComponent *_RenderTransformComponent) {
//     if (!_RenderTransformComponent)
//         return _TransformComponent->worldRect;

//     // Rectangle expandedRect = RectangleUtils::Expand(_TransformComponent->worldRect,
//     //                                                 _RenderTransformComponent->rectOffset);

//     // Rectangle movedRect = RectangleUtils::Move(expandedRect,
//     _RenderTransformComponent->offset); return Rectangle{0, 0, 0, 0};
// }

// void RenderSystem::ShowFPS(bool _Value) { m_ShowFPS = _Value; }
// } // namespace feyerverx::ecs