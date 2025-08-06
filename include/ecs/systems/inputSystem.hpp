// Copyright 2025 Valentin Namleev

#pragma once

#include "../../events/eventQueue.hpp"
#include "../../events/inputEvent.hpp"
#include "../components/inputComponent.hpp"
#include "../components/layoutComponent.hpp"
#include "../components/renderTransformComponent.hpp"
#include "../components/transformComponent.hpp"
#include "../registry.hpp"
#include "../system.hpp"

#include <Rectangle.hpp>
#include <Vector2.hpp>

#include <raylib.h>

namespace turnip::ecs {
class InputSystem : protected ISystem {
public:
    InputSystem(Registry &_Registry, events::EventQueue &_EventQueue)
        : ISystem(_Registry), m_EventQueue(_EventQueue) {}
    ~InputSystem() = default;
    void Update(float _DeltaTime) override {
        raylib::Vector2 mousePosition = GetMousePosition();
        bool pressed = IsMouseButtonPressed(MouseButton::MOUSE_LEFT_BUTTON);
        bool released = IsMouseButtonReleased(MouseButton::MOUSE_LEFT_BUTTON);

        bool isMoving = mousePosition != m_MousePreviousPosition;
        bool wasPressed = m_WasMouseDown;
        bool isPressed = pressed || (wasPressed && !released);
        bool isDragging = isMoving && isPressed;

        if (pressed) {
            m_EventQueue.Push({ecs::NullEntity, events::InputEventType::PRESSED, mousePosition,
                               MouseButton::MOUSE_LEFT_BUTTON});
        }

        if (released) {
            m_EventQueue.Push({ecs::NullEntity, events::InputEventType::RELEASED, mousePosition,
                               MouseButton::MOUSE_LEFT_BUTTON});
        }

        if (isDragging) {
            m_EventQueue.Push({ecs::NullEntity, events::InputEventType::DRAGGED, mousePosition});
        } else {
            m_EventQueue.Push({ecs::NullEntity, events::InputEventType::MOVED, mousePosition});
        }
    }

private:
    raylib::Vector2 m_MousePreviousPosition;
    events::EventQueue &m_EventQueue;
    bool m_WasMouseDown;
};
}
