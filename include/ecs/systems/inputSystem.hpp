// Copyright 2025 Valentin Namleev

#pragma once

#include "../../events/eventQueue.hpp"
#include "../../events/inputEvent.hpp"
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
    InputSystem(Registry &_Registry, events::EventQueue &_EventQueue);

    void Update(float _DeltaTime) override;

private:
    raylib::Vector2 m_MousePreviousPosition;
    events::EventQueue &m_EventQueue;
    bool m_WasMouseDown;

    void FetchMouseEvents();
    void FetchKeyboardEvents();
};
}
