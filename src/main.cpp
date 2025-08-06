// Copyright 2025 Valentin Namleev

#include "./engine.hpp"

int main() {
    turnip::Engine engine(640, 480, "Turnip");

    turnip::UISceneBuilder &sceneBuilder = engine.UISceneBuilder();
    turnip::ResourcesManager &resourcesManager = engine.ResourcesManager();

    turnip::ecs::EntityID sceneRoot = sceneBuilder.CreateScene(turnip::LRTB{10, 10, 10, 10});
    // sceneBuilder.CreateImage(sceneRoot, turnip::Resources::GetDefaultTexture("turnip"));

    turnip::ecs::EntityID stackH = sceneBuilder.CreateStack(
        sceneRoot, turnip::ecs::StackType::HORIZONTAL, turnip::ecs::StackContentType::CENTER, 10);

    for (int i = 0; i < 10; ++i) {
        turnip::ecs::EntityID stackV = sceneBuilder.CreateStack(
            stackH, turnip::ecs::StackType::VERTICAL, turnip::ecs::StackContentType::CENTER, 10);

        for (int j = 0; j < 10; ++j) {
            sceneBuilder.CreateImage(stackV, resourcesManager.GetDefaultTexture(), {0, 0, 0, 0},
                                     BLUE);
        }
    }

    // turnip::ecs::EntityID text = sceneBuilder.CreateText(
    //     sceneRoot, "HELLO!", turnip::Resources::GetFont("PlayfairDisplay"), 64, 5, WHITE);

    engine.Run();

    return 0;
}
