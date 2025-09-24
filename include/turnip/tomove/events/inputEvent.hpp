// Copyright 2025 Valentin Namleev

#pragma once

#include "./turnip/ecs/registry.hpp"

#include <Vector2.hpp>

namespace turnip::events {
enum class InputEventType { PRESSED = 0, RELEASED = 1, MOVED = 2, DRAGGED = 3 };

struct InputEvent {
    ecs::EntityID target;
    InputEventType type;
    Vector2 position;
    int key;
};
} // namespace turnip::events