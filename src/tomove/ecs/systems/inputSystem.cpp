// Copyright 2025 Valentin Namleev

#include "./turnip/ecs/systems/inputSystem.hpp"

namespace turnip::ecs {
InputSystem::InputSystem(Registry &_Registry, events::EventQueue &_EventQueue)
    : ISystem(_Registry), m_EventQueue(_EventQueue) {}

void InputSystem::Update([[maybe_unused]] float _DeltaTime) {
    FetchMouseEvents();
    FetchKeyboardEvents();
}

void InputSystem::FetchMouseEvents() {
    raylib::Vector2 mousePosition = GetMousePosition();
    bool pressed = IsMouseButtonPressed(MouseButton::MOUSE_LEFT_BUTTON);
    bool released = IsMouseButtonReleased(MouseButton::MOUSE_LEFT_BUTTON);

    bool isMoving = mousePosition != m_MousePreviousPosition;
    bool wasPressed = m_WasMouseDown;
    bool isPressed = pressed || (wasPressed && !released);
    bool isDragging = isMoving && isPressed;

    if (pressed) {
        m_EventQueue.Push({ecs::NullEntityID, events::InputEventType::PRESSED, mousePosition,
                           MouseButton::MOUSE_LEFT_BUTTON});
    }

    if (released) {
        m_EventQueue.Push({ecs::NullEntityID, events::InputEventType::RELEASED, mousePosition,
                           MouseButton::MOUSE_LEFT_BUTTON});
    }

    if (isDragging) {
        m_EventQueue.Push({ecs::NullEntityID, events::InputEventType::DRAGGED, mousePosition,
                           MouseButton::MOUSE_LEFT_BUTTON});
    } else {
        m_EventQueue.Push({ecs::NullEntityID, events::InputEventType::MOVED, mousePosition,
                           MouseButton::MOUSE_LEFT_BUTTON});
    }
}

void InputSystem::FetchKeyboardEvents() {}
}
