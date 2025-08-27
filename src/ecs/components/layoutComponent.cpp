// Copyright 2025 Valentin Namleev

#include "./ecs/components/layoutComponent.hpp"

namespace turnip::ecs {
LayoutComponent::LayoutComponent(Size _Size, LRTB _Margin, LRTB _Padding)
    : size(_Size), margin(_Margin), padding(_Padding) {}
} // namespace turnip::ecs
