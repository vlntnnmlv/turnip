// Copyright 2025 Valentin Namleev

#include "feyerverx/axis.hpp"

namespace feyerverx {
float &AxisHelper::getRectPosition(Rectangle &rect, Axis axis) {
    return axis == Axis::HORIZONTAL ? rect.x : rect.y;
}

float &AxisHelper::getRectSize(Rectangle &rect, Axis axis) {
    return axis == Axis::HORIZONTAL ? rect.width : rect.height;
}

float AxisHelper::getLayoutSize(const Size &size, Axis axis) {
    return axis == Axis::HORIZONTAL ? size.width : size.height;
}

std::optional<float> AxisHelper::getLayoutConstraintSize(const Size &size, Axis axis, bool min) {
    return axis == Axis::HORIZONTAL ? (min ? size.minWidth : size.maxWidth)
                                    : (min ? size.minHeight : size.maxHeight);
}

SizeType AxisHelper::getLayoutSizeType(const feyerverx::Size &size, Axis axis) {
    return (axis == Axis::HORIZONTAL) ? size.axisX : size.axisY;
}

float AxisHelper::getPadding(const LRTB &padding, Axis axis, bool isStart) {
    return axis == Axis::HORIZONTAL ? (isStart ? padding.left : padding.right)
                                    : (isStart ? padding.top : padding.bottom);
}

float AxisHelper::getMargin(const LRTB &margin, Axis axis, bool isStart) {
    return axis == Axis::HORIZONTAL ? (isStart ? margin.left : margin.right)
                                    : (isStart ? margin.top : margin.bottom);
}
} // namespace feyerverx
