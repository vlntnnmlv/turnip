// Copyright 2025 Valentin Namleev

#include "./turnip/ecs/systems/renderSystem.hpp"

namespace turnip::ecs {
RenderSystem::RenderSystem(Registry &_Registry, std::unique_ptr<raylib::Window> &_Window)
    : ISystem(_Registry), m_Window(_Window) {}

void RenderSystem::Update([[maybe_unused]] float _DeltaTime) { Render(); }

void RenderSystem::SetBackgroundColor(Color _Color) { m_BackgroundColor = _Color; }

void RenderSystem::Render() {
    BeginDrawing();
    m_Window->ClearBackground(m_BackgroundColor);

    RenderSprites();
    RenderTexts();
    RenderGraphs();

    // RenderDebug();

    // ---
    DrawText(std::to_string(GetFPS()).c_str(), 0, 0, 24, RED);
    // ---

    EndDrawing();
}

void RenderSystem::RenderSprites() {
    std::vector<EntityID> toRender = m_Registry.With<TransformComponent, SpriteComponent>();
    std::sort(toRender.begin(), toRender.end());

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

        Rectangle renderRect = GetRenderRect(e, transformComponent);

        DrawTextureNPatch(spriteComponent->texture, patchInfo, renderRect, {0, 0}, 0, color);
    }
}

void RenderSystem::RenderTexts() {
    std::vector<EntityID> toRender = m_Registry.With<TransformComponent, TextComponent>();

    for (EntityID e : toRender) {
        TransformComponent *transformComponent = m_Registry.GetComponent<TransformComponent>(e);
        TextComponent *textComponent = m_Registry.GetComponent<TextComponent>(e);
        ColorComponent *colorComponent = m_Registry.GetComponent<ColorComponent>(e);

        Color color = colorComponent ? colorComponent->color : Color{255, 255, 255, 255};

        Vector2 textSize = textComponent->font.MeasureText(
            textComponent->text.c_str(), textComponent->fontSize, textComponent->spacing);

        Rectangle renderRect = GetRenderRect(e, transformComponent);
        Vector2 center = RectangleUtils::Center(renderRect);

        Vector2 textPosition = Vector2{center.x - textSize.x * 0.5f, center.y - textSize.y * 0.5f};

        textComponent->font.DrawText(textComponent->text.c_str(), textPosition,
                                     textComponent->fontSize, textComponent->spacing, color);
    }
}

static float map(float _V, float _MinFrom, float _MaxFrom, float _MinTo, float _MaxTo) {
    return ((_V - _MinFrom) / (_MaxFrom - _MinFrom)) * (_MaxTo - _MinTo) + _MinTo;
}

static Vector2 map(Vector2 _V, Vector2 _MinFrom, Vector2 _MaxFrom, Vector2 _MinTo, Vector2 _MaxTo) {
    return Vector2{
        map(_V.x, _MinFrom.x, _MaxFrom.x, _MinTo.x, _MaxTo.x),
        map(_V.y, _MinFrom.y, _MaxFrom.y, _MinTo.y, _MaxTo.y),
    };
}

void RenderSystem::RenderGraphs() {
    std::vector<EntityID> toRender = m_Registry.With<TransformComponent, GraphComponent>();

    for (EntityID e : toRender) {
        TransformComponent *transformComponent = m_Registry.GetComponent<TransformComponent>(e);
        GraphComponent *graphComponent = m_Registry.GetComponent<GraphComponent>(e);

        if (graphComponent->valuesInTime.size() < 2)
            continue;

        Rectangle renderRect = GetRenderRect(e, transformComponent);

        ColorComponent *colorComponent = m_Registry.GetComponent<ColorComponent>(e);
        raylib::Color color = colorComponent ? colorComponent->color : WHITE;

        float minValue = graphComponent->minValue();
        float maxValue = graphComponent->maxValue();
        float minTime = graphComponent->valuesInTime.begin()->first;
        float maxTime = std::prev(graphComponent->valuesInTime.end(), 1)->first;

        float minX = renderRect.x;
        float maxX = renderRect.x + renderRect.width;

        float minY = renderRect.y + renderRect.height;
        float maxY = renderRect.y;

        Vector2 start;
        Vector2 end;
        for (size_t i = 0; i < graphComponent->valuesInTime.size() - 1; i++) {
            start = map(Vector2{graphComponent->valuesInTime[i].first,
                                graphComponent->valuesInTime[i].second},
                        Vector2{minTime, minValue}, Vector2{maxTime, maxValue}, Vector2{minX, minY},
                        Vector2{maxX, maxY});
            end = map(Vector2{graphComponent->valuesInTime[i + 1].first,
                              graphComponent->valuesInTime[i + 1].second},
                      Vector2{minTime, minValue}, Vector2{maxTime, maxValue}, Vector2{minX, minY},
                      Vector2{maxX, maxY});
            DrawLineEx(start, end, 3, color);
        }

        DrawCircle(end.x, end.y, 5, color);
    }
}

void RenderSystem::RenderDebug() {
    std::vector<EntityID> toRender = m_Registry.With<TransformComponent>();
    for (EntityID e : toRender) {
        TransformComponent *transformComponent = m_Registry.GetComponent<TransformComponent>(e);

        DrawRectangleLinesEx(GetRenderRect(e, transformComponent), 1, RED);
    }
}

Rectangle RenderSystem::GetRenderRect(EntityID _EntityID, TransformComponent *_TransformComponent) {
    RenderTransformComponent *renderTransformComponent =
        m_Registry.GetComponent<RenderTransformComponent>(_EntityID);

    if (!renderTransformComponent)
        return _TransformComponent->worldRect.Rect();

    Rectangle expandedRect = RectangleUtils::Expand(_TransformComponent->worldRect.Rect(),
                                                    renderTransformComponent->rectOffset);

    Rectangle movedRect = RectangleUtils::Move(expandedRect, renderTransformComponent->offset);
    return movedRect;
}
} // namespace turnip::ecs