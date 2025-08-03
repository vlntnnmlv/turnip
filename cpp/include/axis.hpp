// Copyright 2025 Valentin Namleev

#pragma once

#include <Rectangle.hpp>

#include "./lrtb.hpp"
#include "./size.hpp"

namespace turnip {
enum Axis { HORIZONTAL = 0, VERTICAL = 1 };

struct AxisHelper {
  static float &GetRectPosition(Rectangle &_Rect, Axis _Axis) {
    return _Axis == Axis::HORIZONTAL ? _Rect.x : _Rect.y;
  }

  static float &GetRectSize(Rectangle &_Rect, Axis _Axis) {
    return _Axis == Axis::HORIZONTAL ? _Rect.width : _Rect.height;
  }

  static float GetLayoutSize(const turnip::Size &_Size, Axis _Axis) {
    return _Axis == Axis::HORIZONTAL ? _Size.width : _Size.height;
  }

  static turnip::SizeType GetLayoutSizeType(const turnip::Size &_Size,
                                            Axis _Axis) {
    return (_Axis == Axis::HORIZONTAL) ? _Size.axisX : _Size.axisY;
  }

  static float GetPadding(const LRTB &_Padding, Axis _Axis, bool _IsStart) {
    return _Axis == Axis::HORIZONTAL
               ? (_IsStart ? _Padding.left : _Padding.right)
               : (_IsStart ? _Padding.top : _Padding.bottom);
  }

  static float GetMargin(const LRTB &_Margin, Axis _Axis, bool _IsStart) {
    return _Axis == Axis::HORIZONTAL
               ? (_IsStart ? _Margin.left : _Margin.right)
               : (_IsStart ? _Margin.top : _Margin.bottom);
  }
};
} // namespace turnip
