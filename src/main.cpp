// Copyright 2025 Valentin Namleev

#include "./turnip/colorUtils.hpp"
#include "./turnip/ecs/components/childrenComponent.hpp"
#include "./turnip/ecs/components/colorComponent.hpp"
#include "./turnip/ecs/components/graphComponent.hpp"
#include "./turnip/ecs/components/textComponent.hpp"
#include "./turnip/ecs/components/transformComponent.hpp"
#include "./turnip/ecs/entity.hpp"
#include "./turnip/engine.hpp"

#include <map>
#include <random>
#include <string>

int main() {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_real_distribution<std::mt19937::result_type> dist10(1.0, 3.0);

    turnip::Engine engine(860, 640, "Turnip");

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

    turnip::ecs::EntityID panelLeft = createPanelWithBg(stackH, turnip::Size{.width = 240},
                                                        {0, 0, 0, 0}, {4, 4, 4, 4}, {70, 84, 109});

    turnip::ecs::EntityID panelRight =
        createPanelWithBg(stackH, turnip::Size{turnip::SizeType::FILL, turnip::SizeType::FILL},
                          {0, 0, 0, 0}, {4, 4, 4, 4}, {70, 84, 109});

    turnip::ecs::EntityID graph = turnip::ecs::NullEntity;
    {
        graph = sceneBuilder.CreateNode(
            panelRight, turnip::Size{turnip::SizeType::FILL, turnip::SizeType::FILL},
            {10, 10, 10, 10});
        engine.Registry().AddComponent<turnip::ecs::GraphComponent>(graph);
        engine.Registry().AddComponent<turnip::ecs::ColorComponent>(
            graph, raylib::Color{186, 10, 10, 255});
    }

    turnip::ecs::EntityID stockbookStack = sceneBuilder.CreateStack(
        panelLeft, turnip::ecs::StackType::HORIZONTAL, turnip::ecs::StackContentType::START, 2);

    auto createStockBook = [&createPanelWithBg,
                            &sceneBuilder](turnip::ecs::EntityID _Parent) -> turnip::ecs::EntityID {
        turnip::ecs::EntityID stockbookRoot =
            createPanelWithBg(_Parent, turnip::Size{turnip::SizeType::FILL, turnip::SizeType::FILL},
                              {0, 0, 0, 0}, {3, 3, 3, 3}, {87, 100, 122});

        turnip::ecs::EntityID stockbook = sceneBuilder.CreateStack(
            stockbookRoot, turnip::ecs::StackType::VERTICAL, turnip::ecs::StackContentType::END, 2,
            turnip::Size{turnip::SizeType::FILL, turnip::SizeType::FILL}, {0, 0, 0, 0},
            {2, 2, 2, 2});

        return stockbook;
    };

    turnip::ecs::EntityID stockbookBuy = createStockBook(stockbookStack);
    turnip::ecs::EntityID stockbookSell = createStockBook(stockbookStack);

    static int bidsAndAsksCount = 0;
    auto onBidAdded = [&graph, &engine]() {
        engine.Registry().GetComponent<turnip::ecs::GraphComponent>(graph)->valuesInTime.push_back(
            std::pair<float, float>(static_cast<float>(GetTime()),
                                    static_cast<float>(bidsAndAsksCount)));
    };

    auto addToStockBook = [&createPanelWithBg, &onBidAdded, &sceneBuilder, &resourcesManager](
                              turnip::ecs::EntityID _StockBook, raylib::Color _Color,
                              raylib::Color _TextColor) -> turnip::ecs::EntityID {
        turnip::ecs::EntityID bid = createPanelWithBg(
            _StockBook,
            turnip::Size{turnip::SizeType::FILL, turnip::SizeType::FILL, 0, 0, 0, 0, 10, 0, 30, 0},
            {0, 0, 0, 0}, {0, 0, 0, 0}, _Color);

        sceneBuilder.CreateText(bid, "BID", resourcesManager.GetFont("martian_mono"), 14, 5,
                                _TextColor);
        onBidAdded();
        return bid;
    };

    static float time = 0;
    engine.AddUpdateStep([&addToStockBook, &stockbookSell](float _DeltaTime) {
        time += _DeltaTime;
        if (time > 4.5f) {
            addToStockBook(stockbookSell, {102, 114, 134}, {218, 221, 226});
            time = 0;
            bidsAndAsksCount++;
        }
    });

    static float time2 = 0;
    engine.AddUpdateStep([&addToStockBook, &stockbookBuy, &dist10, &rng](float _DeltaTime) {
        time2 += _DeltaTime;
        if (time2 > dist10(rng)) {
            addToStockBook(stockbookBuy, {102, 114, 134}, {218, 221, 226});
            time2 = 0;
            bidsAndAsksCount++;
        }
    });

    engine.Run();

    return 0;
}
