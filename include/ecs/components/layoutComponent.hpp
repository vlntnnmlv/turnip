// Copyright 2025 Valentin Namleev

#pragma once

#include "../component.hpp"
#include "./lrtb.hpp"
#include "./size.hpp"

namespace turnip::ecs {
struct LayoutComponent : IComponent {
    Size size;
    LRTB margin;
    LRTB padding;

    LayoutComponent(Size _Size = Size{SizeType::FILL, SizeType::FILL},
                    LRTB _Margin = LRTB{0, 0, 0, 0}, LRTB _Padding = LRTB{0, 0, 0, 0});
};
} // namespace turnip::ecs
