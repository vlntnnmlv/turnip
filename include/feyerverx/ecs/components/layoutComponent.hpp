// Copyright 2025 Valentin Namleev

#pragma once

#include "feyerverx/ecs/component.hpp"
#include "feyerverx/lrtb.hpp"
#include "feyerverx/size.hpp"

namespace feyerverx::ecs {
struct LayoutComponent : IComponent {
    Size size;
    LRTB margin;
    LRTB padding;

    LayoutComponent(Size _Size = Size(), LRTB _Margin = LRTB{0, 0, 0, 0},
                    LRTB _Padding = LRTB{0, 0, 0, 0});
};
} // namespace feyerverx::ecs
