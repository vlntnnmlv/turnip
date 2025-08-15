// Copyright 2025 Valentin Namleev

#include "./colorUtils.hpp"
#include "./ecs/components/colorComponent.hpp"
#include "./ecs/components/textComponent.hpp"
#include "./ecs/entity.hpp"
#include "./engine.hpp"

#include <string>

int main() {
    turnip::Engine engine(640, 480, "Turnip");

    turnip::UISceneBuilder &sceneBuilder = engine.UISceneBuilder();
    turnip::ResourcesManager &resourcesManager = engine.ResourcesManager();
    turnip::ecs::EntityID sceneRoot = sceneBuilder.CreateScene();

    turnip::ecs::EntityID stackH = sceneBuilder.CreateStack(
        sceneRoot, turnip::ecs::StackType::HORIZONTAL, turnip::ecs::StackContentType::END, 5);

    turnip::ecs::EntityID panelLeft = sceneBuilder.CreateStack(
        stackH, turnip::ecs::StackType::HORIZONTAL, turnip::ecs::StackContentType::CENTER, 2,
        turnip::Size{turnip::SizeType::START, turnip::SizeType::FILL, 160, 0});

    turnip::ecs::EntityID img = sceneBuilder.CreateImage(
        panelLeft, resourcesManager.GetDefaultTexture(), {0, 0, 0, 0}, RED);

    turnip::ecs::EntityID panelRight = sceneBuilder.CreateNode(stackH);

    turnip::ecs::EntityID img2 = sceneBuilder.CreateImage(
        panelRight, resourcesManager.GetDefaultTexture(), {0, 0, 0, 0}, BLUE);

    // turnip::ecs::EntityID text = sceneBuilder.CreateText(
    //     sceneRoot, "HELLO!", turnip::Resources::GetFont("PlayfairDisplay"), 64, 5, WHITE);

    engine.Run();

    return 0;
}
