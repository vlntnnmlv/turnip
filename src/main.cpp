// Copyright 2025 Valentin Namleev

#include "./colorUtils.hpp"
#include "./engine.hpp"

int main() {
    turnip::Engine engine(640, 480, "Turnip");

    turnip::UISceneBuilder &sceneBuilder = engine.UISceneBuilder();
    turnip::ResourcesManager &resourcesManager = engine.ResourcesManager();
    turnip::ecs::EntityID sceneRoot = sceneBuilder.CreateScene();

    turnip::ecs::EntityID stackH = sceneBuilder.CreateStack(
        sceneRoot, turnip::ecs::StackType::HORIZONTAL, turnip::ecs::StackContentType::CENTER, 5);

    turnip::ecs::EntityID panelLeft = sceneBuilder.CreateNode(
        stackH, turnip::Size{turnip::SizeType::FILL, turnip::SizeType::FILL, 200, 10});

    turnip::ecs::EntityID panelRight = sceneBuilder.CreateNode(
        stackH, turnip::Size{turnip::SizeType::FILL, turnip::SizeType::FILL, 200, 10});

    turnip::ecs::EntityID imgLeft =
        sceneBuilder.CreateImage(panelLeft, resourcesManager.GetDefaultTexture(), {0, 0, 0, 0},
                                 turnip::ColorUtils::GetColorShade(0.6f, RED));

    turnip::ecs::EntityID imgRight =
        sceneBuilder.CreateImage(panelRight, resourcesManager.GetDefaultTexture(), {0, 0, 0, 0},
                                 turnip::ColorUtils::GetColorShade(0.6f, BLUE));

    // int dimension = 10;
    // for (int x = 0; x < dimension; ++x) {
    //     turnip::ecs::EntityID stackV = sceneBuilder.CreateStack(
    //         stackH, turnip::ecs::StackType::VERTICAL, turnip::ecs::StackContentType::CENTER,
    //         5);

    //     for (int y = 0; y < dimension; ++y) {
    //         sceneBuilder.CreateImage(
    //             stackV, resourcesManager.GetDefaultTexture(), {0, 0, 0, 0},
    //             turnip::ColorUtils::GetColorShade((x + y) * (1.0f / (2 * dimension)), RED));
    //     }
    // }

    // turnip::ecs::EntityID text = sceneBuilder.CreateText(
    //     sceneRoot, "HELLO!", turnip::Resources::GetFont("PlayfairDisplay"), 64, 5, WHITE);

    engine.Run();

    return 0;
}
