// Copyright 2025 Valentin Namleev

#pragma once

#include "feyerverx/ecs/component.hpp"
#include "feyerverx/rectangleOffset.hpp"
#include "feyerverx/size.hpp"

namespace feyerverx::ecs {
struct LayoutComponent : IComponent {
    Size size;
    RectangleOffset margin;
    RectangleOffset padding;

    LayoutComponent(Size _Size = Size(), RectangleOffset _Margin = RectangleOffset{0, 0, 0, 0},
                    RectangleOffset _Padding = RectangleOffset{0, 0, 0, 0});
};
} // namespace feyerverx::ecs
