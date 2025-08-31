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

        Rectangle renderRect =
            turnip::ecs::RenderSystem::GetRenderRect(transformComponent, renderTransformComponent);

        turnip::ecs::ColorComponent *colorComponent =
            _Registry.GetComponent<turnip::ecs::ColorComponent>(e);
        raylib::Color color = colorComponent ? colorComponent->color : WHITE;

        Value minValue = graphComponent->minValue();
        Value maxValue = graphComponent->maxValue();
        Time minTime = graphComponent->valuesInTime.begin()->first;
        Time maxTime = std::prev(graphComponent->valuesInTime.end(), 1)->first;

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

int main() {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_real_distribution<float> dist10(1.0, 20.0);

    turnip::Engine engine(860, 640, "Turnip");
    engine.ResourcesManager().SetResourcesDirectory(std::filesystem::absolute("../../resources"));

    engine.RenderSystem().RegisterRenderer(
        {typeid(turnip::ecs::TransformComponent), typeid(GraphComponent)},
        [](std::vector<turnip::ecs::EntityID> &_ToRender, turnip::ecs::Registry &_Registry) {
            RenderGraphs(_ToRender, _Registry);
        });

    turnip::UISceneBuilder &sceneBuilder = engine.UISceneBuilder();
    turnip::ResourcesManager &resourcesManager = engine.ResourcesManager();
    turnip::ecs::EntityID sceneRoot = sceneBuilder.CreateScene({5, 5, 5, 5});

    auto createPanelWithBg = [&sceneBuilder,
                              &resourcesManager](turnip::ecs::EntityID _Parent, turnip::Size _Size,
                                                 turnip::LRTB _Margin, turnip::LRTB _Padding,
                                                 raylib::Color _Color) -> turnip::ecs::EntityID {
        turnip::ecs::EntityID panel = sceneBuilder.CreateNode(_Parent, _Size, _Margin, _Padding);

        sceneBuilder.CreateImage(panel, resourcesManager.GetSmoothCornerTexture(5), {5, 5, 5, 5},
                                 _Color);

        return panel;
    };

    turnip::ecs::EntityID stackH = sceneBuilder.CreateStack(
        sceneRoot, turnip::ecs::StackType::HORIZONTAL, turnip::ecs::StackContentType::START, 2);

    turnip::ecs::EntityID panelLeft = createPanelWithBg(
        stackH, turnip::Size{.axisX = turnip::SizeType::START, .width = 240}, turnip::LRTB{},
        turnip::LRTB{4, 4, 4, 4}, raylib::Color{70, 84, 109, 255});

    turnip::ecs::EntityID panelRight = createPanelWithBg(
        stackH, turnip::Size{.axisX = turnip::SizeType::FILL, .axisY = turnip::SizeType::FILL},
        turnip::LRTB{}, turnip::LRTB{4, 4, 4, 4}, raylib::Color{70, 84, 109, 255});

    turnip::ecs::EntityID panelRightRight = createPanelWithBg(
        stackH, turnip::Size{.axisX = turnip::SizeType::FILL, .axisY = turnip::SizeType::FILL},
        turnip::LRTB{}, turnip::LRTB{4, 4, 4, 4}, raylib::Color{70, 84, 109, 255});

    turnip::ecs::EntityID rightRightHStack = sceneBuilder.CreateStack(
        panelRightRight, turnip::ecs::StackType::HORIZONTAL, turnip::ecs::StackContentType::CENTER,
        4, turnip::Size{}, turnip::LRTB{}, turnip::LRTB{4, 4, 4, 4});

    auto red = createPanelWithBg(rightRightHStack,
                                 turnip::Size{.axisY = turnip::SizeType::END, .height = 0},
                                 turnip::LRTB{}, turnip::LRTB{}, RED);

    auto green = createPanelWithBg(rightRightHStack,
                                   turnip::Size{.axisY = turnip::SizeType::END, .height = 0},
                                   turnip::LRTB{}, turnip::LRTB{}, GREEN);

    turnip::ecs::EntityID graph = turnip::ecs::NullEntity;
    {
        graph = sceneBuilder.CreateNode(
            panelRight, turnip::Size{turnip::SizeType::FILL, turnip::SizeType::FILL},
            turnip::LRTB{10, 10, 10, 10});
        engine.Registry().AddComponent<GraphComponent>(graph);
        engine.Registry().AddComponent<turnip::ecs::ColorComponent>(
            graph, raylib::Color{186, 10, 10, 255});
    }

    turnip::ecs::EntityID stockbookStack = sceneBuilder.CreateStack(
        panelLeft, turnip::ecs::StackType::HORIZONTAL, turnip::ecs::StackContentType::START, 2);

    auto createStockBook = [&createPanelWithBg,
                            &sceneBuilder](turnip::ecs::EntityID _Parent) -> turnip::ecs::EntityID {
        turnip::ecs::EntityID stockbookRoot = createPanelWithBg(
            _Parent, turnip::Size{turnip::SizeType::FILL, turnip::SizeType::FILL}, turnip::LRTB{},
            turnip::LRTB{3, 3, 3, 3}, raylib::Color{87, 100, 122, 255});

        turnip::ecs::EntityID stockbook = sceneBuilder.CreateStack(
            stockbookRoot, turnip::ecs::StackType::VERTICAL, turnip::ecs::StackContentType::END, 2,
            turnip::Size{turnip::SizeType::FILL, turnip::SizeType::FILL}, turnip::LRTB{},
            turnip::LRTB{2, 2, 2, 2});

        return stockbook;
    };

    turnip::ecs::EntityID stockbookBuy = createStockBook(stockbookStack);
    turnip::ecs::EntityID stockbookSell = createStockBook(stockbookStack);

    static int bidsAndAsksCount = 0;
    auto onBidAdded = [&graph, &engine]() {
        engine.Registry().GetComponent<GraphComponent>(graph)->valuesInTime.push_back(
            std::pair<float, float>(static_cast<float>(GetTime()),
                                    static_cast<float>(bidsAndAsksCount)));
    };

    auto onSellBidAdded = [&red, &engine]() {
        engine.Registry().GetComponent<turnip::ecs::LayoutComponent>(red)->size.height += 10;
    };

    auto onBuyBidAdded = [&green, &engine]() {
        engine.Registry().GetComponent<turnip::ecs::LayoutComponent>(green)->size.height += 10;
    };

    auto addToStockBook =
        [&createPanelWithBg, &onBidAdded, &sceneBuilder, &resourcesManager, &onSellBidAdded,
         &onBuyBidAdded](turnip::ecs::EntityID _StockBook, raylib::Color _Color,
                         raylib::Color _TextColor, bool _Buy) -> turnip::ecs::EntityID {
        turnip::ecs::EntityID bid = createPanelWithBg(_StockBook,
                                                      turnip::Size{.axisX = turnip::SizeType::FILL,
                                                                   .axisY = turnip::SizeType::START,
                                                                   .height = 30,
                                                                   .minHeight = 10,
                                                                   .maxHeight = 50},
                                                      turnip::LRTB{}, turnip::LRTB{}, _Color);

        sceneBuilder.CreateText(bid, "BID", resourcesManager.GetFont("martian_mono"), 24, 5,
                                _TextColor);
        onBidAdded();
        if (_Buy)
            onBuyBidAdded();
        else
            onSellBidAdded();

        return bid;
    };

    static float time = 0;
    engine.AddUpdateStep([&addToStockBook, &stockbookSell](float _DeltaTime) {
        time += _DeltaTime;
        if (time > 0.2f) {
            addToStockBook(stockbookSell, {102, 114, 134}, {218, 221, 226}, false);
            time = 0;
            bidsAndAsksCount++;
        }
    });

    float time2 = 0;
    float target = dist10(rng);
    engine.AddUpdateStep(
        [&addToStockBook, &stockbookBuy, &dist10, &rng, &time2, &target](float _DeltaTime) {
            time2 += _DeltaTime;
            if (time2 > target) {
                addToStockBook(stockbookBuy, {102, 114, 134}, {218, 221, 226}, true);
                time2 = 0;
                target = dist10(rng);
                bidsAndAsksCount++;
            }
        });

    engine.Run();

    return 0;
}
