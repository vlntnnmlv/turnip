// Copyright 2025 Valentin Namleev

#include "./engine.hpp"

int main() {
    turnip::Engine engine = turnip::Engine(640, 480, "Turnip");

    turnip::UISceneBuilder &sceneBuilder = engine.UISceneBuilder();

    turnip::ecs::EntityID sceneRoot = sceneBuilder.CreateScene(turnip::LRTB{10, 10, 10, 10});

    sceneBuilder.CreateNode(sceneRoot);

    engine.Run();

    return 0;
}
