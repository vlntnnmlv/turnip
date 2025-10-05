// Copyright 2025 Valentin Namleev

#pragma once

#include "feyerverx/ecs/entity.hpp"
#include "feyerverx/ecs/registry.hpp"
#include "feyerverx/math/vector.hpp"

namespace feyerverx::events {
enum class InputEventType { PRESSED = 0, RELEASED = 1, MOVED = 2, DRAGGED = 3 };

struct InputEvent {
    ecs::EntityID target;
    InputEventType type;
    Vector2f position;
    int key;
};
} // namespace feyerverx::events