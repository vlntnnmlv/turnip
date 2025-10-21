// Copyright October 2025 Valentin Namleev

#include "feyerverx/eventSink.hpp"

namespace feyerverx {

EventHandler<SDL_Keycode> EventSink::OnKeyDown{};
EventHandler<SDL_Keycode> EventSink::OnKeyUp{};

EventHandler<SDL_MouseMotionEvent> EventSink::OnMouseMoved{};
EventHandler<SDL_MouseButtonEvent> EventSink::OnMouseDown{};
EventHandler<SDL_MouseButtonEvent> EventSink::OnMouseUp{};

EventHandler<Vector2f> EventSink::OnWindowResized{};
}