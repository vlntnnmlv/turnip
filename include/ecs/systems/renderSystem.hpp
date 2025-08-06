// Copyright 2025 Valentin Namleev

#pragma once

#include "../../rectangleUtils.hpp"
#include "../components/colorComponent.hpp"
#include "../components/renderTransformComponent.hpp"
#include "../components/spriteComponent.hpp"
#include "../components/textComponent.hpp"
#include "../components/transformComponent.hpp"
#include "../registry.hpp"
#include "../system.hpp"
#include <raylib-cpp.hpp>

#include <memory>

namespace turnip::ecs {
class RenderSystem : protected ISystem {
public:
    RenderSystem(Registry &_Registry, std::unique_ptr<raylib::Window> &_Window)
        : ISystem(_Registry), m_Window(_Window) {}

    void Update(float _DeltaTime) override { Render(); }
    void SetBackgroundColor(Color _Color) { m_BackgroundColor = _Color; }

private:
    void Render() {
        BeginDrawing();
        m_Window->ClearBackground(m_BackgroundColor);

        RenderSprites();
        RenderTexts();

        // ---
        DrawText(std::to_string(GetFPS()).c_str(), 0, 0, 24, RED);
        // ---

        EndDrawing();
    }

    void RenderSprites() {
        std::vector<EntityID> toRender = m_Registry.With<TransformComponent, SpriteComponent>();

        for (EntityID e : toRender) {
            TransformComponent *transformComponent = m_Registry.GetComponent<TransformComponent>(e);
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

            DrawTextureNPatch(spriteComponent->texture, patchInfo,
                              GetRenderRect(e, transformComponent), {0, 0}, 0, color);
        }
    }

    void RenderTexts() {
        std::vector<EntityID> toRender = m_Registry.With<TransformComponent, TextComponent>();

        for (EntityID e : toRender) {
            TransformComponent *transformComponent = m_Registry.GetComponent<TransformComponent>(e);
            TextComponent *textComponent = m_Registry.GetComponent<TextComponent>(e);
            ColorComponent *colorComponent = m_Registry.GetComponent<ColorComponent>(e);

            Color color = colorComponent ? colorComponent->color : Color{255, 255, 255, 255};

            Vector2 textSize = textComponent->font.MeasureText(
                textComponent->text.c_str(), textComponent->fontSize,
                textComponent->spacing); // textComponent->fontSize);

            Rectangle renderRect = GetRenderRect(e, transformComponent);
            Vector2 center = RectangleUtils::Center(renderRect);

            Vector2 textPosition =
                Vector2{center.x - textSize.x * 0.5f, center.y - textSize.y * 0.5f};

            textComponent->font.DrawText(textComponent->text.c_str(), textPosition,
                                         textComponent->fontSize, textComponent->spacing, color);
        }
    }

    Rectangle GetRenderRect(EntityID _EntityID, TransformComponent *_TransformComponent) {
        RenderTransformComponent *renderTransformComponent =
            m_Registry.GetComponent<RenderTransformComponent>(_EntityID);

        if (!renderTransformComponent)
            return _TransformComponent->worldRect;

        Rectangle expandedRect = RectangleUtils::Expand(_TransformComponent->worldRect,
                                                        renderTransformComponent->rectOffset);

        return RectangleUtils::Move(expandedRect, renderTransformComponent->offset);
    }

    Color m_BackgroundColor;
    std::unique_ptr<raylib::Window> &m_Window;
};
} // namespace turnip::ecs