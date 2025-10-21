// Copyright 2025 Valentin Namleev

#include <feyerverx/fey.hpp>

#include <feyerverx/rectangle.hpp>

#include <feyerverx/ecs/components/spriteComponent.hpp>
#include <feyerverx/ecs/components/transformComponent.hpp>
#include <feyerverx/ecs/entity.hpp>
#include <feyerverx/ecs/systems/uiSystem.hpp>

using namespace feyerverx;

void addSpriteToRender(ecs::Scene &mainScene, Texture &t, Rectangle rect) {
    ecs::Entity e = mainScene.addEntity();
    e.addComponent<ecs::TransformComponent>(rect);
    e.addComponent<ecs::SpriteComponent>(t);
}

void add(ecs::Scene &scene, Texture &tt, float a, float b, float c, float d) {
    auto e = scene.addEntity();
    e.addComponent<ecs::TransformComponent>(Rectangle{a, b, c, d});
    e.addComponent<ecs::SpriteComponent>(tt);
}

int main() {
    const Specification specification = {"Fey", {800, 600}};
    auto feyResult = Fey::create(specification);
    if (!feyResult) {
        Logger::instance().log(LogLevel::Error, feyResult.error().message);
        return 1;
    }

    Fey fey = std::move(feyResult.value());

    ecs::Scene &mainScene = fey.addScene("main_scene", {0, 1, 0, 1});
    auto t = fey.assetManager().getTexture("bush");
    add(mainScene, t, 0, 0, 650, 30);
    add(mainScene, t, 654, 122, 400, 630);
    add(mainScene, t, 50, 444, 870, 70);
    add(mainScene, t, 344, 69, 100, 230);
    add(mainScene, t, 90, 6, 100, 100);

    ecs::Scene &mainScene2 = fey.addScene("main_scene2", {0, 1, 0, 1});
    auto t2 = fey.assetManager().getTexture("bean");
    add(mainScene2, t2, 0, 0, 653, 30);
    add(mainScene2, t2, 64, 122, 400, 60);
    add(mainScene2, t2, 504, 44, 870, 70);
    add(mainScene2, t2, 344, 68, 100, 230);
    add(mainScene2, t2, 99, 6, 10, 100);

    fey.run();

    return 0;
}