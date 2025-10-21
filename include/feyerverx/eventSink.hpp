// Copyright October 2025 Valentin Namleev

#pragma once
#include "SDL3/SDL_events.h"
#include "SDL3/SDL_keycode.h"

#include "feyerverx/eventHandler.hpp"
#include "math/vector.hpp"

namespace feyerverx {
class EventSink {
public:
    static EventHandler<SDL_Keycode> OnKeyDown;
    static EventHandler<SDL_Keycode> OnKeyUp;

    static EventHandler<SDL_MouseMotionEvent> OnMouseMoved;
    static EventHandler<SDL_MouseButtonEvent> OnMouseDown;
    static EventHandler<SDL_MouseButtonEvent> OnMouseUp;

    static EventHandler<Vector2f> OnWindowResized;
};
}