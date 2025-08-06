// Copyright 2025 Valentin Namleev

#pragma once

#include "../components/inputComponent.hpp"
#include "../components/layoutComponent.hpp"
#include "../components/renderTransformComponent.hpp"
#include "../components/transformComponent.hpp"
#include "../registry.hpp"
#include "../system.hpp"
#include <Rectangle.hpp>
#include <Vector2.hpp>

namespace turnip::ecs {
class InputSystem : protected ISystem {
public:
    InputSystem(Registry &_Registry) : ISystem(_Registry) {}
    ~InputSystem() = default;
    void Update(float _DeltaTime) override {
        Vector2 mouse = GetMousePosition();
        bool down = IsMouseButtonDown(MOUSE_LEFT_BUTTON);
        bool released = IsMouseButtonReleased(MOUSE_LEFT_BUTTON);

        for (const auto &e : m_Registry.With<TransformComponent, RenderTransformComponent>()) {
            auto *r = m_Registry.GetComponent<RenderTransformComponent>(e);

            if (m_Registry.GetComponent<TransformComponent>(e)->worldRect.CheckCollision(mouse)) {
                r->rectOffset.left = 5;
                r->rectOffset.right = 5;
                r->rectOffset.top = 5;
                r->rectOffset.bottom = 5;
            } else {
                r->rectOffset.left = 0;
                r->rectOffset.right = 0;
                r->rectOffset.top = 0;
                r->rectOffset.bottom = 0;
            }
        }
    }

private:
};
}
