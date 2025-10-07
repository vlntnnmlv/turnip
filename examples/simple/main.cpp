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
    try {
        feyerverx::Fey fey("Fey", 800, 600);
        feyerverx::ecs::Scene &mainScene = fey.addScene("main_scene", true);

        auto ui = std::make_unique<feyerverx::ecs::UISystem>();
        ui->m_size = {800, 600};
        mainScene.addSystem(std::move(ui));

        feyerverx::uiBuilder &builder = mainScene.builder();
        const auto root = builder.root();
        const auto stack = builder.stack(root, feyerverx::ecs::StackType::HORIZONTAL,
                                         feyerverx::ecs::StackContentType::CENTER, 10);

        auto t = feyerverx::Texture(fey.assetManager().getTexture("bush"));
        builder.image(stack, t);
        builder.image(stack, t);
        builder.image(stack, t);
        builder.image(stack, t);
        builder.image(stack, t);

        fey.run();
    } catch (feyerverx::FeyError &e) {
        std::print("{}", e.what());
    }
    return 0;
}