// Copyright 2025 Valentin Namleev

#pragma once

#include "./turnip/ecs/components/layoutComponent.hpp"
#include "./turnip/ecs/components/renderTransformComponent.hpp"
#include "./turnip/ecs/components/transformComponent.hpp"
#include "./turnip/ecs/registry.hpp"
#include "./turnip/ecs/system.hpp"
#include "./turnip/events/eventQueue.hpp"
#include "./turnip/events/inputEvent.hpp"

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
