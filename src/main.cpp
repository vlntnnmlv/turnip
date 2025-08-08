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

    // turnip::ecs::EntityID panelLeft = sceneBuilder.CreateNode(
    //     stackH, turnip::Size{turnip::SizeType::CENTER, turnip::SizeType::FILL, 150, 10});

    // turnip::ecs::EntityID panelRight = sceneBuilder.CreateNode(
    //     stackH, turnip::Size{turnip::SizeType::CENTER, turnip::SizeType::FILL, 150, 10});

    // turnip::ecs::EntityID imgLeft =
    //     sceneBuilder.CreateImage(panelLeft, resourcesManager.GetFrameTexture(5), {5, 5, 5, 5},
    //                              turnip::ColorUtils::GetColorShade(0.6f, RED));

    // turnip::ecs::EntityID imgRight =
    //     sceneBuilder.CreateImage(panelRight, resourcesManager.GetFrameTexture(5), {5, 5, 5, 5},
    //                              turnip::ColorUtils::GetColorShade(0.6f, BLUE));

    // turnip::ecs::EntityID button = sceneBuilder.CreateButton(imgRight, [&engine, imgLeft]() {
    //     static bool pressed;
    //     engine.Registry().GetComponent<turnip::ecs::ColorComponent>(imgLeft)->color =
    //         pressed ? YELLOW : GREEN;

    //     pressed = !pressed;
    // });

    int dimension = 10;
    for (int x = 0; x < dimension; ++x) {
        turnip::ecs::EntityID stackV = sceneBuilder.CreateStack(
            stackH, turnip::ecs::StackType::VERTICAL, turnip::ecs::StackContentType::CENTER, 5);

        for (int y = 0; y < dimension; ++y) {
            turnip::ecs::EntityID img = sceneBuilder.CreateImage(
                stackV, resourcesManager.GetDefaultTexture(), {0, 0, 0, 0},
                turnip::ColorUtils::GetColorShade(1 - (x + y) * (1.0f / (2 * dimension)), RED));

            turnip::ecs::EntityID text = sceneBuilder.CreateText(
                img, "0", resourcesManager.GetFont("PlayfairDisplay"), 32, 5, WHITE);

            turnip::ecs::EntityID button = sceneBuilder.CreateButton(img, [&engine, text]() {
                turnip::ecs::TextComponent *textComponent =
                    engine.Registry().GetComponent<turnip::ecs::TextComponent>(text);

                textComponent->text = std::to_string(std::atoi(textComponent->text.c_str()) + 1);
            });
        }
    }

    // turnip::ecs::EntityID text = sceneBuilder.CreateText(
    //     sceneRoot, "HELLO!", turnip::Resources::GetFont("PlayfairDisplay"), 64, 5, WHITE);

    engine.Run();

    return 0;
}
