// Copyright 2025 Valentin Namleev

#include "feyerverx/ecs/components/layoutComponent.hpp"

namespace feyerverx::ecs {
LayoutComponent::LayoutComponent(Size _Size, RectangleOffset _Margin, RectangleOffset _Padding)
    : size(_Size), margin(_Margin), padding(_Padding) {}
} // namespace feyerverx::ecs
