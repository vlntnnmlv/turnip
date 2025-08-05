// Copyright 2025 Valentin Namleev

#include "./engine.hpp"
#include "./resources.hpp"

int main() {
    turnip::Engine engine = turnip::Engine(640, 480, "Turnip");

    turnip::UISceneBuilder &sceneBuilder = engine.UISceneBuilder();

    turnip::ecs::EntityID sceneRoot = sceneBuilder.CreateScene(turnip::LRTB{10, 10, 10, 10});
    turnip::ecs::EntityID stackH = sceneBuilder.CreateStack(
        sceneRoot, turnip::ecs::StackType::HORIZONTAL, turnip::ecs::StackContentType::CENTER, 10);

    for (int i = 0; i < 10; ++i) {
        turnip::ecs::EntityID stackV = sceneBuilder.CreateStack(
            stackH, turnip::ecs::StackType::VERTICAL, turnip::ecs::StackContentType::CENTER, 10);

        for (int j = 0; j < 10; ++j) {
            sceneBuilder.CreateImage(stackV, turnip::Resources::GetTexture("crate"));
        }
    }

    engine.Run();

    return 0;
}
