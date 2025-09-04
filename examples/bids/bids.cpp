// Copyright 2025 Valentin Namleev

#include <turnip/colorUtils.hpp>
#include <turnip/ecs/components/childrenComponent.hpp>
#include <turnip/ecs/components/colorComponent.hpp>
#include <turnip/ecs/components/textComponent.hpp>
#include <turnip/ecs/components/transformComponent.hpp>
#include <turnip/ecs/entity.hpp>
#include <turnip/engine.hpp>

#include <filesystem>
#include <map>
#include <random>
#include <string>

#include "./graphComponent.hpp"
#include "./orderBook.hpp"

static float map(float _V, float _MinFrom, float _MaxFrom, float _MinTo, float _MaxTo) {
    return ((_V - _MinFrom) / (_MaxFrom - _MinFrom)) * (_MaxTo - _MinTo) + _MinTo;
}

static Vector2 map(Vector2 _V, Vector2 _MinFrom, Vector2 _MaxFrom, Vector2 _MinTo, Vector2 _MaxTo) {
    return Vector2{
        map(_V.x, _MinFrom.x, _MaxFrom.x, _MinTo.x, _MaxTo.x),
        map(_V.y, _MinFrom.y, _MaxFrom.y, _MinTo.y, _MaxTo.y),
    };
}

void RenderGraphs(std::vector<turnip::ecs::EntityID> &_ToRender, turnip::ecs::Registry &_Registry) {
    for (turnip::ecs::EntityID e : _ToRender) {
        turnip::ecs::TransformComponent *transformComponent =
            _Registry.GetComponent<turnip::ecs::TransformComponent>(e);

        turnip::ecs::RenderTransformComponent *renderTransformComponent =
            _Registry.GetComponent<turnip::ecs::RenderTransformComponent>(e);

        GraphComponent *graphComponent = _Registry.GetComponent<GraphComponent>(e);

        if (graphComponent->valuesInTime.size() < 2)
            continue;

        raylib::Rectangle renderRect =
            turnip::ecs::RenderSystem::GetRenderRect(transformComponent, renderTransformComponent);

        turnip::ecs::ColorComponent *colorComponent =
            _Registry.GetComponent<turnip::ecs::ColorComponent>(e);
        raylib::Color color = colorComponent ? colorComponent->color : WHITE;

        Value minValue = graphComponent->minValue();
        Value maxValue = graphComponent->maxValue();
        Time minTime = graphComponent->minTime();
        Time maxTime = graphComponent->maxTime();

        float minX = renderRect.x;
        float maxX = renderRect.x + renderRect.width;

        float minY = renderRect.y + renderRect.height;
        float maxY = renderRect.y;

        Vector2 start;
        Vector2 end;

        // TODO: Add to render system
        // TODO: Use Vulkan/OpenGL instead of raylib

        bool invertX = graphComponent->settings.invertX;
        auto startI = invertX ? graphComponent->valuesInTime.size() - 2
                              : graphComponent->firstIndexInTimeSpan();
        auto endI = invertX ? graphComponent->firstIndexInTimeSpan()
                            : graphComponent->valuesInTime.size() - 1;
        auto step = invertX ? -1 : 1;

        bool drawnInvert = false;
        for (size_t i = startI; i != endI; i += step) {
            start = map(Vector2{graphComponent->valuesInTime[i].first,
                                graphComponent->valuesInTime[i].second},
                        Vector2{invertX ? maxTime : minTime, minValue},
                        Vector2{invertX ? minTime : maxTime, maxValue}, Vector2{minX, minY},
                        Vector2{maxX, maxY});
            if (invertX && !drawnInvert) {
                DrawCircle(start.x, start.y, 5, color);
                drawnInvert = true;
            }
            end = map(Vector2{graphComponent->valuesInTime[i + 1].first,
                              graphComponent->valuesInTime[i + 1].second},
                      Vector2{invertX ? maxTime : minTime, minValue},
                      Vector2{invertX ? minTime : maxTime, maxValue}, Vector2{minX, minY},
                      Vector2{maxX, maxY});
            DrawLineEx(start, end, graphComponent->settings.lineThickness, color);
        }

        if (!invertX)
            DrawCircle(end.x, end.y, 5, color);

        if (graphComponent->settings.showAxis) {
            DrawLineEx(Vector2{renderRect.x, renderRect.y},
                       Vector2{renderRect.x, renderRect.y + renderRect.height},
                       graphComponent->settings.lineThickness, color);

            DrawLineEx(Vector2{renderRect.x, renderRect.y + renderRect.height},
                       Vector2{renderRect.x + renderRect.width, renderRect.y + renderRect.height},
                       graphComponent->settings.lineThickness, color);
        }
    }
}

int main() {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::normal_distribution<float> dist01(0.0, 1.0);

    turnip::Engine engine(860, 640, "Turnip");
    engine.ResourcesManager().SetResourcesDirectory(std::filesystem::absolute("../../resources"));

    engine.RenderSystem().SetBackgroundColor({52, 67, 94, 255});
    engine.RenderSystem().RegisterRenderer(
        {typeid(turnip::ecs::TransformComponent), typeid(GraphComponent)},
        [](std::vector<turnip::ecs::EntityID> &_ToRender, turnip::ecs::Registry &_Registry) {
            RenderGraphs(_ToRender, _Registry);
        });

    turnip::UISceneBuilder &sceneBuilder = engine.UISceneBuilder();
    turnip::ResourcesManager &resourcesManager = engine.ResourcesManager();
    turnip::ecs::EntityID sceneRoot = sceneBuilder.CreateScene({5, 5, 5, 5});
    auto stack = sceneBuilder.CreateStack(sceneRoot, turnip::ecs::StackType::HORIZONTAL,
                                          turnip::ecs::StackContentType::CENTER, 4);

    GraphComponent *sellGraph;
    GraphComponent *buyGraph;
    {
        auto root = sceneBuilder.CreateNode(stack);
        sceneBuilder.CreateImage(root, resourcesManager.GetSmoothCornerTexture(4),
                                 turnip::LRTB{4, 4, 4, 4}, raylib::Color{70, 84, 109, 255});

        sceneBuilder.CreateText(root, "SELL", resourcesManager.GetFont("martian_mono"), 24, 5,
                                WHITE,
                                turnip::Size{.axisY = turnip::SizeType::START, .height = 50});

        auto sellGraphNode = sceneBuilder.CreateNode(root, turnip::Size{}, turnip::LRTB{4, 4, 4, 4},
                                                     turnip::LRTB{4, 4, 4, 4});
        engine.Registry().AddComponent<GraphComponent>(sellGraphNode,
                                                       GraphSettings{.lineThickness = 2});
        engine.Registry().AddComponent<turnip::ecs::ColorComponent>(
            sellGraphNode, raylib::Color{201, 255, 213, 255});

        sellGraph = engine.Registry().GetComponent<GraphComponent>(sellGraphNode);
    }

    {
        auto root = sceneBuilder.CreateNode(stack);
        sceneBuilder.CreateImage(root, resourcesManager.GetSmoothCornerTexture(4),
                                 turnip::LRTB{4, 4, 4, 4}, raylib::Color{70, 84, 109, 255});

        sceneBuilder.CreateText(root, "BUY", resourcesManager.GetFont("martian_mono"), 24, 5, WHITE,
                                turnip::Size{.axisY = turnip::SizeType::START, .height = 50});

        auto buyGraphNode = sceneBuilder.CreateNode(root, turnip::Size{}, turnip::LRTB{4, 4, 4, 4},
                                                    turnip::LRTB{4, 4, 4, 4});
        engine.Registry().AddComponent<GraphComponent>(
            buyGraphNode, GraphSettings{.lineThickness = 2, .invertX = true});
        engine.Registry().AddComponent<turnip::ecs::ColorComponent>(
            buyGraphNode, raylib::Color{255, 206, 213, 255});

        buyGraph = engine.Registry().GetComponent<GraphComponent>(buyGraphNode);
    }

    static float sellValue = 0;
    static float buyValue = 0;
    static bool sellButtonPressed = false;
    static bool buyButtonPressed = false;

    {
        auto buttonsStack = sceneBuilder.CreateStack(
            stack, turnip::ecs::StackType::VERTICAL, turnip::ecs::StackContentType::CENTER, 4,
            turnip::Size{.axisX = turnip::SizeType::END, .width = 100});

        sceneBuilder.CreateButton(
            buttonsStack, []() { sellButtonPressed = true; },
            resourcesManager.GetSmoothCornerTexture(4), turnip::LRTB{4, 4, 4, 4},
            raylib::Color{201, 255, 213, 255});

        sceneBuilder.CreateButton(
            buttonsStack, []() { buyButtonPressed = true; },
            resourcesManager.GetSmoothCornerTexture(4), turnip::LRTB{4, 4, 4, 4},
            raylib::Color{255, 206, 213, 255});
    }

    engine.AddUpdateStep(
        [&sellGraph, &buyGraph, &dist01, &rng](float _Time, [[maybe_unused]] float _DeltaTime) {
            if (sellButtonPressed)
                sellValue += dist01(rng);

            if (buyButtonPressed)
                buyValue += dist01(rng);

            sellGraph->valuesInTime.emplace_back(_Time, sellValue);
            buyGraph->valuesInTime.emplace_back(_Time, buyValue);
            sellButtonPressed = false;
            buyButtonPressed = false;
        });

    // auto createPanelWithBg = [&sceneBuilder,
    //                           &resourcesManager](turnip::ecs::EntityID _Parent,
    //                           turnip::Size _Size,
    //                                              turnip::LRTB _Margin, turnip::LRTB
    //                                              _Padding, raylib::Color _Color) ->
    //                                              turnip::ecs::EntityID {
    //     turnip::ecs::EntityID panel = sceneBuilder.CreateNode(_Parent, _Size, _Margin,
    //     _Padding);

    //     sceneBuilder.CreateImage(panel, resourcesManager.GetSmoothCornerTexture(5),
    //     {5, 5, 5, 5},
    //                              _Color);

    //     return panel;
    // };

    // turnip::ecs::EntityID stackH = sceneBuilder.CreateStack(
    //     sceneRoot, turnip::ecs::StackType::HORIZONTAL,
    //     turnip::ecs::StackContentType::START, 2);

    // createPanelWithBg(stackH, turnip::Size{.axisX = turnip::SizeType::START,
    // .width = 240},
    //                   turnip::LRTB{}, turnip::LRTB{4, 4, 4, 4},
    //                   raylib::Color{70, 84, 109, 255});

    engine.Run();

    return 0;
}
