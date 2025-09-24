// Copyright 2025 Valentin Namleev

#include "./turnip/ecs/components/renderTransformComponent.hpp"

namespace turnip::ecs {
RenderTransformComponent::RenderTransformComponent(Vector2 _Offset, LRTB _RectOffset,
                                                   Vector2 _Scale)
    : offset(_Offset), rectOffset(_RectOffset), scale(_Scale) {}
} // namespace turnip::ecs
