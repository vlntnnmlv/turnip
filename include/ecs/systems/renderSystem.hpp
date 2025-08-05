// Copyright 2025 Valentin Namleev

#pragma once

#include "../components/colorComponent.hpp"
#include "../components/spriteComponent.hpp"
#include "../components/transformComponent.hpp"
#include "../registry.hpp"
#include "../system.hpp"

#include <memory>

namespace turnip::ecs {
class RenderSystem : protected ISystem {
public:
    RenderSystem(Registry &_Registry, std::unique_ptr<raylib::Window> &_Window)
        : ISystem(_Registry), m_Window(_Window) {}

    void Update(float _DeltaTime) override { Render(); }

private:
    void Render() {
        BeginDrawing();
        m_Window->ClearBackground(raylib::WHITE);

        std::vector<EntityID> toRender = m_Registry.With<TransformComponent, SpriteComponent>();

        for (EntityID e : toRender) {
            TransformComponent *transformComponent = m_Registry.GetComponent<TransformComponent>(e);

            SpriteComponent *spriteComponent = m_Registry.GetComponent<SpriteComponent>(e);

            ColorComponent *colorComponent = m_Registry.GetComponent<ColorComponent>(e);

            ::Color color = colorComponent ? colorComponent->color : ::Color{255, 255, 255, 255};

            NPatchInfo patchInfo =
                NPatchInfo{Rectangle{0, 0, static_cast<float>(spriteComponent->texture.width),
                                     static_cast<float>(spriteComponent->texture.height)},
                           static_cast<int>(spriteComponent->patch.left),
                           static_cast<int>(spriteComponent->patch.top),
                           static_cast<int>(spriteComponent->patch.right),
                           static_cast<int>(spriteComponent->patch.bottom),
                           NPATCH_NINE_PATCH};

            DrawTextureNPatch(spriteComponent->texture, patchInfo, transformComponent->worldRect,
                              {0, 0}, 0, color);
        }

        // ---
        DrawText(std::to_string(GetFPS()).c_str(), 0, 0, 24, RED);
        // ---

        EndDrawing();
    }

    std::unique_ptr<raylib::Window> &m_Window;
};
} // namespace turnip::ecs