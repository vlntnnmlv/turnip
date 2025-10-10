// Copyright 2025 Valentin Namleev

#include "feyerverx/ecs/systems/uiSystem.hpp"

#include <print>

#include <feyerverx/ecs/components/spriteComponent.hpp>
#include <feyerverx/ecs/components/transformComponent.hpp>
#include <feyerverx/ecs/entity.hpp>

#include <feyerverx/error.hpp>
#include <feyerverx/fey.hpp>
#include <feyerverx/rectangle.hpp>

void addSpriteToRender(feyerverx::ecs::Scene &mainScene, feyerverx::Texture &t,
                       feyerverx::Rectangle rect) {
    feyerverx::ecs::Entity e = mainScene.addEntity();
    e.addComponent<feyerverx::ecs::TransformComponent>(rect);
    e.addComponent<feyerverx::ecs::SpriteComponent>(t);
}

int main() {
    // auto feyMaybe = feyerverx::Fey::create("Fey", 800, 600);
    // if (const feyerverx::Error *err = std::get_if<feyerverx::Error>(&feyMaybe)) {
    //     feyerverx::Logger::instance().log(feyerverx::LogLevel::Error, err->message);
    //     return 1;
    // }
    //
    // feyerverx::Fey fey = std::move(std::get<feyerverx::Fey>(feyMaybe));
    // feyerverx::ecs::Scene &mainScene = fey.addScene("main_scene", true);
    //
    // mainScene.addSystem<feyerverx::ecs::UISystem>();
    //
    // feyerverx::uiBuilder &builder = mainScene.builder();
    // const auto root = builder.root();
    // const auto stack = builder.stack(root, feyerverx::ecs::StackType::HORIZONTAL,
    //                                  feyerverx::ecs::StackContentType::CENTER, 10);
    //
    // auto t = feyerverx::Texture(fey.assetManager().getTexture("bush"));
    // builder.image(stack, t);
    // builder.image(stack, t);
    // builder.image(stack, t);
    // builder.image(stack, t);
    // builder.image(stack, t);
    //
    // fey.run();

    return 0;
}