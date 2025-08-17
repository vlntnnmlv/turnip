// Copyright 2025 Valentin Namleev

#include "./colorUtils.hpp"
#include "./ecs/components/colorComponent.hpp"
#include "./ecs/components/textComponent.hpp"
#include "./ecs/entity.hpp"
#include "./engine.hpp"

#include <map>
#include <string>

int main() {
    turnip::Engine engine(860, 640, "Turnip");

    turnip::UISceneBuilder &sceneBuilder = engine.UISceneBuilder();
    turnip::ResourcesManager &resourcesManager = engine.ResourcesManager();
    turnip::ecs::EntityID sceneRoot = sceneBuilder.CreateScene({5, 5, 5, 5});

    auto createPanelWithBg = [&sceneBuilder,
                              &resourcesManager](turnip::ecs::EntityID _Parent, turnip::Size _Size,
                                                 turnip::LRTB _Margin, turnip::LRTB _Padding,
                                                 raylib::Color _Color) -> turnip::ecs::EntityID {
        turnip::ecs::EntityID panel = sceneBuilder.CreateNode(_Parent, _Size, _Margin, _Padding);

        turnip::ecs::EntityID img = sceneBuilder.CreateImage(
            panel, resourcesManager.GetSmoothCornerTexture(5), {5, 5, 5, 5}, _Color);

        return panel;
    };

    turnip::ecs::EntityID stackH = sceneBuilder.CreateStack(
        sceneRoot, turnip::ecs::StackType::HORIZONTAL, turnip::ecs::StackContentType::START, 2);

    turnip::ecs::EntityID panelLeft = createPanelWithBg(
        stackH, turnip::Size{turnip::SizeType::START, turnip::SizeType::FILL, 240}, {0, 0, 0, 0},
        {4, 4, 4, 4}, {70, 84, 109});

    turnip::ecs::EntityID panelRight =
        createPanelWithBg(stackH, turnip::Size{turnip::SizeType::FILL, turnip::SizeType::FILL},
                          {0, 0, 0, 0}, {4, 4, 4, 4}, {70, 84, 109});

    sceneBuilder.CreateText(panelRight, "Coming soon", resourcesManager.GetFont("martian_mono"), 48,
                            5, {169, 177, 187});

    turnip::ecs::EntityID stockbookStack = sceneBuilder.CreateStack(
        panelLeft, turnip::ecs::StackType::HORIZONTAL, turnip::ecs::StackContentType::START, 2);

    auto createStockBook = [&createPanelWithBg, &sceneBuilder, &resourcesManager](
                               turnip::ecs::EntityID _Parent) -> turnip::ecs::EntityID {
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

    auto addToStockBook = [&createPanelWithBg, &sceneBuilder, &resourcesManager](
                              turnip::ecs::EntityID _StockBook, raylib::Color _Color,
                              raylib::Color _TextColor) -> turnip::ecs::EntityID {
        turnip::ecs::EntityID bid = createPanelWithBg(
            _StockBook, turnip::Size{turnip::SizeType::FILL, turnip::SizeType::FILL}, {0, 0, 0, 0},
            {0, 0, 0, 0}, _Color);

        sceneBuilder.CreateText(bid, "BID", resourcesManager.GetFont("martian_mono"), 14, 5,
                                _TextColor);
        return bid;
    };

    static float time = 0;
    engine.AddUpdateStep([&addToStockBook, &stockbookSell](float _DeltaTime) {
        time += _DeltaTime;
        if (time > 2) {
            addToStockBook(stockbookSell, {242, 84, 91}, {84, 242, 235});
            time = 0;
        }
    });

    static float time2 = 0;
    engine.AddUpdateStep([&addToStockBook, &stockbookBuy](float _DeltaTime) {
        time2 += _DeltaTime;
        if (time2 > 3) {
            addToStockBook(stockbookBuy, {80, 114, 60}, {94, 60, 114});
            time2 = 0;
        }
    });

    engine.Run();

    return 0;
}
