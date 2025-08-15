// Copyright 2025 Valentin Namleev

#include "./colorUtils.hpp"
#include "./ecs/components/colorComponent.hpp"
#include "./ecs/components/textComponent.hpp"
#include "./ecs/entity.hpp"
#include "./engine.hpp"

#include <string>

int main() {
    turnip::Engine engine(620, 480, "Turnip");

    turnip::UISceneBuilder &sceneBuilder = engine.UISceneBuilder();
    turnip::ResourcesManager &resourcesManager = engine.ResourcesManager();
    turnip::ecs::EntityID sceneRoot = sceneBuilder.CreateScene();

    turnip::ecs::EntityID stackH = sceneBuilder.CreateStack(
        sceneRoot, turnip::ecs::StackType::HORIZONTAL, turnip::ecs::StackContentType::END, 5);

    int dimension = 10;
    for (int x = 0; x < dimension; ++x) {
        turnip::ecs::EntityID stackV = sceneBuilder.CreateStack(
            stackH, turnip::ecs::StackType::VERTICAL, turnip::ecs::StackContentType::CENTER, 5);

        for (int y = 0; y < dimension; ++y) {
            turnip::ecs::EntityID panel = sceneBuilder.CreateNode(
                stackV, turnip::Size{turnip::SizeType::FILL, turnip::SizeType::FILL}, {0, 0, 0, 0},
                {0, 0, 0, 0});

            turnip::ecs::EntityID text = sceneBuilder.CreateText(
                panel, "0", resourcesManager.GetFont("PlayfairDisplay"), 32, 5, WHITE);

            turnip::ecs::EntityID button = sceneBuilder.CreateButton(
                panel,
                [&engine, text]() {
                    turnip::ecs::TextComponent *textComponent =
                        engine.Registry().GetComponent<turnip::ecs::TextComponent>(text);

                    textComponent->text =
                        std::to_string(std::atoi(textComponent->text.c_str()) + 1);
                },
                resourcesManager.GetDefaultTexture(), {0, 0, 0, 0},
                turnip::ColorUtils::GetColorShade(1 - (x + y) * (1.0f / (2 * dimension)), RED));
        }
    }

    // turnip::ecs::EntityID text = sceneBuilder.CreateText(
    //     sceneRoot, "HELLO!", turnip::Resources::GetFont("PlayfairDisplay"), 64, 5, WHITE);

    engine.Run();

    return 0;
}
