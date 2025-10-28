// Copyright 2025 Valentin Namleev

#include <feyerverx/fey.hpp>

#include <feyerverx/ecs/components/spriteComponent.hpp>
#include <feyerverx/ecs/components/transformComponent.hpp>
#include <feyerverx/ecs/entity.hpp>
#include <feyerverx/rectangle.hpp>

#include <feyerverx/ecs/systems/uiSystem.hpp>

using namespace feyerverx;

void add(const ecs::Scene &scene, Texture &tt, float a, float b, float c, float d) {
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

    ecs::Scene &mainScene = fey.addScene("main_scene", {0, 1, 0, 1}, {69, 69, 69, 255});

    mainScene.addSystem<ecs::UISystem>(Vector2f{800, 600}, fey.eventManager());

    auto devil = fey.assetManager().getTexture("devil");

    const auto root = mainScene.builder().root();
    mainScene.builder().image(root, devil, {0, 0, 0, 0}, {255, 255, 255, 255},
                              Size{SizeType::CENTER, SizeType::CENTER, 64, 64});

    fey.run();

    return 0;
}