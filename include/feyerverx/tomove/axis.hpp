// Copyright 2025 Valentin Namleev

#pragma once

#include <Rectangle.hpp>

#include "./turnip/lrtb.hpp"
#include "./turnip/size.hpp"

namespace feyerverx {
enum class Axis { HORIZONTAL = 0, VERTICAL = 1 };

struct AxisHelper {
    static float &GetRectPosition(Rectangle &_Rect, Axis _Axis);
    static float &GetRectSize(Rectangle &_Rect, Axis _Axis);
    static float GetLayoutSize(const feyerverx::Size &_Size, Axis _Axis);
    static std::optional<float> GetLayoutConstraintSize(const turnip::Size &_Size, Axis _Axis,
                                                        bool _Min);
    static feyerverx::SizeType GetLayoutSizeType(const feyerverx::Size &_Size, Axis _Axis);
    static float GetPadding(const LRTB &_Padding, Axis _Axis, bool _IsStart);
    static float GetMargin(const LRTB &_Margin, Axis _Axis, bool _IsStart);
};
} // namespace feyerverx
