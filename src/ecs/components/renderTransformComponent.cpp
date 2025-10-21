// Copyright 2025 Valentin Namleev

#include "feyerverx/ecs/components/renderTransformComponent.hpp"

namespace feyerverx::ecs {
RenderTransformComponent::RenderTransformComponent(Vector2f _Offset, RectangleOffset _RectOffset,
                                                   Vector2f _Scale)
    : offset(_Offset), rectOffset(_RectOffset), scale(_Scale) {}
} // namespace feyerverx::ecs
