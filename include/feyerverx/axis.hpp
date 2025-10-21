// Copyright 2025 Valentin Namleev

#pragma once

#include "feyerverx/rectangle.hpp"
#include "feyerverx/rectangleOffset.hpp"
#include "feyerverx/size.hpp"

namespace feyerverx {
enum class Axis { HORIZONTAL = 0, VERTICAL = 1 };

struct AxisHelper {
    static float &getRectPosition(Rectangle &rect, Axis axis);
    static float &getRectSize(Rectangle &rect, Axis axis);
    static float getLayoutSize(const Size &size, Axis axis);
    static std::optional<float> getLayoutConstraintSize(const Size &size, Axis axis, bool min);
    static SizeType getLayoutSizeType(const Size &size, Axis axis);
    static float getPadding(const RectangleOffset &padding, Axis axis, bool isStart);
    static float getMargin(const RectangleOffset &margin, Axis axis, bool isStart);
};
} // namespace feyerverx
