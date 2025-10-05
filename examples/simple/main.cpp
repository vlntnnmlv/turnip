// Copyright 2025 Valentin Namleev

#include <print>

#include <feyerverx/ecs/components/spriteComponent.hpp>
#include <feyerverx/ecs/components/transformComponent.hpp>
#include <feyerverx/ecs/entity.hpp>

#include <feyerverx/assetLoader.hpp>
#include <feyerverx/error.hpp>
#include <feyerverx/fey.hpp>
#include <feyerverx/rectangle.hpp>

void addSpriteToRender(feyerverx::ecs::Scene &mainScene, feyerverx::Texture &t,
                       feyerverx::Rectangle rect) {
    feyerverx::ecs::Entity e = mainScene.addEntity();
    e.AddComponent<feyerverx::ecs::TransformComponent>(rect);
    e.AddComponent<feyerverx::ecs::SpriteComponent>(t);
}

int main() {
    try {
        feyerverx::Fey fey("Turnip", 800, 600);
        feyerverx::ecs::Scene &mainScene = fey.addScene(true);

        feyerverx::Texture t = feyerverx::Texture(fey.assetManager().getTexture("bush"));

        fey.run();

    } catch (feyerverx::FeyError &e) {
        std::print("{}", e.what());
    }
    return 0;
}