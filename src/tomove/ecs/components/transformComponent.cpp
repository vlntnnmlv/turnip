// Copyright 2025 Valentin Namleev

#include "./turnip/ecs/components/transformComponent.hpp"

namespace feyerverx::ecs {
TransformComponent::TransformComponent(Rectangle _Rect) : rect(_Rect), worldRect(_Rect) {}
} // namespace feyerverx::ecs
