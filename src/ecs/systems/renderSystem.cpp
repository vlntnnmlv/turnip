// Copyright 2025 Valentin Namleev

#include "./turnip/ecs/systems/renderSystem.hpp"

// TODO: Option to add custom renderers!

namespace turnip::ecs {
RenderSystem::RenderSystem(Registry &_Registry, std::unique_ptr<raylib::Window> &_Window)
    : ISystem(_Registry), m_Window(_Window) {
    RegisterRenderer(
        {typeid(ecs::TransformComponent), typeid(ecs::SpriteComponent)},
        [this](std::vector<EntityID> &_ToRender, [[maybe_unused]] ecs::Registry &_Registry) {
            RenderSprites(_ToRender);
        });

    RegisterRenderer({typeid(ecs::TransformComponent), typeid(ecs::TextComponent)},
                     [this](std::vector<EntityID> &_ToRender,
                            [[maybe_unused]] ecs::Registry &_Registry) { RenderTexts(_ToRender); });

    // RegisterRenderer({typeid(ecs::TransformComponent)},
    //                  [this](std::vector<EntityID> &_ToRender,
    //                         [[maybe_unused]] ecs::Registry &_Registry) { RenderDebug(_ToRender);
    //                         });
}

void RenderSystem::Update([[maybe_unused]] float _DeltaTime) { Render(); }

void RenderSystem::SetBackgroundColor(Color _Color) { m_BackgroundColor = _Color; }

void RenderSystem::RegisterRenderer(const ComponentTypeSet &_ComponentTypeSet,
                                    RenderCallback _RenderCallback) {
    m_ComponentRenderers.emplace_back(_ComponentTypeSet, std::move(_RenderCallback));
}

void RenderSystem::Render() {
    BeginDrawing();
    m_Window->ClearBackground(m_BackgroundColor);

    for (const auto &[componentTypeSet, renderCallback] : m_ComponentRenderers) {
        std::vector<EntityID> toRender = m_Registry.With(componentTypeSet);
        if (toRender.empty())
            continue;

        renderCallback(toRender, m_Registry);
    }

    // RenderGraphs();

    // ---
    DrawText(std::to_string(GetFPS()).c_str(), 0, 0, 24, RED);
    // ---

    EndDrawing();
}

void RenderSystem::RenderSprites(std::vector<EntityID> &_ToRender) {
    std::sort(_ToRender.begin(), _ToRender.end());

    for (EntityID e : _ToRender) {
        TransformComponent *transformComponent = m_Registry.GetComponent<TransformComponent>(e);
        RenderTransformComponent *renderTransformComponent =
            m_Registry.GetComponent<RenderTransformComponent>(e);
        SpriteComponent *spriteComponent = m_Registry.GetComponent<SpriteComponent>(e);
        ColorComponent *colorComponent = m_Registry.GetComponent<ColorComponent>(e);

        Color color = colorComponent ? colorComponent->color : Color{255, 255, 255, 255};

        NPatchInfo patchInfo =
            NPatchInfo{Rectangle{0, 0, static_cast<float>(spriteComponent->texture.width),
                                 static_cast<float>(spriteComponent->texture.height)},
                       static_cast<int>(spriteComponent->patch.left),
                       static_cast<int>(spriteComponent->patch.top),
                       static_cast<int>(spriteComponent->patch.right),
                       static_cast<int>(spriteComponent->patch.bottom),
                       NPATCH_NINE_PATCH};

        Rectangle renderRect = GetRenderRect(transformComponent, renderTransformComponent);

        DrawTextureNPatch(spriteComponent->texture, patchInfo, renderRect, {0, 0}, 0, color);
    }
}

void RenderSystem::RenderTexts(std::vector<EntityID> &_ToRender) {
    for (EntityID e : _ToRender) {
        TransformComponent *transformComponent = m_Registry.GetComponent<TransformComponent>(e);
        TextComponent *textComponent = m_Registry.GetComponent<TextComponent>(e);
        ColorComponent *colorComponent = m_Registry.GetComponent<ColorComponent>(e);

        Color color = colorComponent ? colorComponent->color : Color{255, 255, 255, 255};

        Vector2 textSize = textComponent->font.MeasureText(
            textComponent->text.c_str(), textComponent->fontSize, textComponent->spacing);

        RenderTransformComponent *renderTransformComponent =
            m_Registry.GetComponent<RenderTransformComponent>(e);

        Rectangle renderRect = GetRenderRect(transformComponent, renderTransformComponent);
        Vector2 center = RectangleUtils::Center(renderRect);

        Vector2 textPosition = Vector2{center.x - textSize.x * 0.5f, center.y - textSize.y * 0.5f};

        textComponent->font.DrawText(textComponent->text.c_str(), textPosition,
                                     textComponent->fontSize, textComponent->spacing, color);
    }
}

void RenderSystem::RenderDebug(std::vector<EntityID> &_ToRender) {
    for (EntityID e : _ToRender) {
        TransformComponent *transformComponent = m_Registry.GetComponent<TransformComponent>(e);
        RenderTransformComponent *renderTransformComponent =
            m_Registry.GetComponent<RenderTransformComponent>(e);

        DrawRectangleLinesEx(GetRenderRect(transformComponent, renderTransformComponent), 1, RED);
    }
}

Rectangle RenderSystem::GetRenderRect(TransformComponent *_TransformComponent,
                                      RenderTransformComponent *_RenderTransformComponent) {
    if (!_RenderTransformComponent)
        return _TransformComponent->worldRect.Rect();

    Rectangle expandedRect = RectangleUtils::Expand(_TransformComponent->worldRect.Rect(),
                                                    _RenderTransformComponent->rectOffset);

    Rectangle movedRect = RectangleUtils::Move(expandedRect, _RenderTransformComponent->offset);
    return movedRect;
}
} // namespace turnip::ecs