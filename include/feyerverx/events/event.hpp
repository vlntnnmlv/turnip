// Copyright 2025 Valentin Namleev

#pragma once
#include "SDL3/SDL_keycode.h"
#include "feyerverx/math/vector.hpp"

namespace feyerverx {
struct Event {
    virtual ~Event() = default;
};

struct WindowResizedEvent final : Event {
    Vector2f windowSize;

    explicit WindowResizedEvent(const Vector2f windowSize) : windowSize(windowSize) {}
};

struct KeyboardEvent final : Event {
    enum class Type { PRESSED, RELEASED };

    SDL_Keycode keycode;
    Type type;

    explicit KeyboardEvent(const SDL_Keycode keycode, const Type type)
        : keycode(keycode), type(type) {}
};
} // namespace feyerverx