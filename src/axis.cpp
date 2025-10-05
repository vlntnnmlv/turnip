// Copyright 2025 Valentin Namleev

#include "feyerverx/axis.hpp"

namespace feyerverx {
float &AxisHelper::GetRectPosition(Rectangle &_Rect, Axis _Axis) {
    return _Axis == Axis::HORIZONTAL ? _Rect.x : _Rect.y;
}

float &AxisHelper::GetRectSize(Rectangle &_Rect, Axis _Axis) {
    return _Axis == Axis::HORIZONTAL ? _Rect.width : _Rect.height;
}

float AxisHelper::GetLayoutSize(const feyerverx::Size &_Size, Axis _Axis) {
    return _Axis == Axis::HORIZONTAL ? _Size.width : _Size.height;
}

std::optional<float> AxisHelper::GetLayoutConstraintSize(const feyerverx::Size &_Size, Axis _Axis,
                                                         bool _Min) {
    return _Axis == Axis::HORIZONTAL ? (_Min ? _Size.minWidth : _Size.maxWidth)
                                     : (_Min ? _Size.minHeight : _Size.maxHeight);
}

feyerverx::SizeType AxisHelper::GetLayoutSizeType(const feyerverx::Size &_Size, Axis _Axis) {
    return (_Axis == Axis::HORIZONTAL) ? _Size.axisX : _Size.axisY;
}

float AxisHelper::GetPadding(const LRTB &_Padding, Axis _Axis, bool _IsStart) {
    return _Axis == Axis::HORIZONTAL ? (_IsStart ? _Padding.left : _Padding.right)
                                     : (_IsStart ? _Padding.top : _Padding.bottom);
}

float AxisHelper::GetMargin(const LRTB &_Margin, Axis _Axis, bool _IsStart) {
    return _Axis == Axis::HORIZONTAL ? (_IsStart ? _Margin.left : _Margin.right)
                                     : (_IsStart ? _Margin.top : _Margin.bottom);
}
} // namespace feyerverx
