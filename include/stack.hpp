// Copyright 2025 Valentin Namleev

#pragma once

#include "./node.hpp"
#include "./rectangleUtils.hpp"

#include <algorithm>
#include <numeric>

namespace turnip {
class Stack : public Node {
public:
  enum StackType { VERTICAL = 0, HORIZONTAL = 1 };
  enum ContentType { START = 0, END = 1, CENTER = 2 };

  Stack(const std::string &_ID, StackType _Type, ContentType _ContentType,
        turnip::Size _Size, ::Color _Color)
      : Node(_ID, _Size, _Color), m_Type(_Type), m_ContentType(_ContentType) {}

  static std::shared_ptr<Stack>
  Create(const std::string &_ID, const std::shared_ptr<Node> &_Parent,
         StackType _Type, ContentType _ContentType = ContentType::CENTER,
         turnip::Size _Size = turnip::Size{turnip::SizeType::FILL,
                                           turnip::SizeType::FILL},
         ::Color _Color = {255, 255, 255, 255}) {
    auto stack =
        std::make_shared<Stack>(_ID, _Type, _ContentType, _Size, _Color);
    stack->SetParent(_Parent);
    return stack;
  }

  ~Stack() override = default;

  const float Spacing() const { return m_Spacing; }
  const void SetSpacing(float _Spacing) { m_Spacing = _Spacing; }

  void Measure() override {
    MeasureAxis(m_Type == StackType::VERTICAL ? Axis::VERTICAL
                                              : Axis::HORIZONTAL);
  }

  void Arrange() override {
    ArrangeAxis(m_Type == StackType::VERTICAL ? Axis::VERTICAL
                                              : Axis::HORIZONTAL);
  }

private:
  float m_AlignedSize = 0;

  void MeasureAxis(Axis _Axis) {
    float fillSpace = AxisHelper::GetRectSize(Rect(), _Axis) -
                      AxisHelper::GetPadding(Padding(), _Axis, true) -
                      AxisHelper::GetPadding(Padding(), _Axis, false) -
                      (Children().size() - 1) * m_Spacing;

    int fillChildrenCount = 0;

    for (auto const &child : Children()) {
      fillSpace -= AxisHelper::GetMargin(child->Margin(), _Axis, true) +
                   AxisHelper::GetMargin(child->Margin(), _Axis, false);

      if (AxisHelper::GetLayoutSizeType(child->Size(), _Axis) !=
          SizeType::FILL) {
        fillSpace -= AxisHelper::GetLayoutSize(child->Size(), _Axis);
      } else
        fillChildrenCount += 1;
    }

    Axis orthogonalAxis =
        (_Axis == Axis::HORIZONTAL) ? Axis::VERTICAL : Axis::HORIZONTAL;

    for (auto const &child : Children()) {
      turnip::Size const &size = child->Size();
      Rectangle childRect = child->Rect();

      switch (AxisHelper::GetLayoutSizeType(size, _Axis)) {
      case SizeType::FILL:
        m_AlignedSize += AxisHelper::GetRectSize(childRect, _Axis) =
            fillSpace / fillChildrenCount;
        break;
      default:
        m_AlignedSize += AxisHelper::GetRectSize(childRect, _Axis) =
            AxisHelper::GetLayoutSize(size, _Axis);
        break;
      }

      float orthoAvailable =
          AxisHelper::GetRectSize(Rect(), orthogonalAxis) -
          AxisHelper::GetPadding(Padding(), orthogonalAxis, true) -
          AxisHelper::GetPadding(Padding(), orthogonalAxis, false) -
          AxisHelper::GetMargin(child->Margin(), orthogonalAxis, true) -
          AxisHelper::GetMargin(child->Margin(), orthogonalAxis, false);

      switch (AxisHelper::GetLayoutSizeType(size, orthogonalAxis)) {
      case SizeType::FILL:
        AxisHelper::GetRectSize(childRect, orthogonalAxis) = orthoAvailable;
        break;
      default:
        AxisHelper::GetRectSize(childRect, orthogonalAxis) =
            AxisHelper::GetLayoutSize(size, orthogonalAxis);
        break;
      }

      child->SetRect(childRect);
    }
  }

  void ArrangeAxis(Axis _Axis) {
    Axis orthogonalAxis =
        (_Axis == Axis::HORIZONTAL) ? Axis::VERTICAL : Axis::HORIZONTAL;

    for (auto const &child : Children()) {
      turnip::Size size = child->Size();
      Rectangle childRect = child->Rect();

      float offset;
      switch (AxisHelper::GetLayoutSizeType(size, _Axis)) {
      case SizeType::FILL:
      case SizeType::START:
        offset = AxisHelper::GetPadding(Padding(), _Axis, true);
        break;
      case SizeType::END:
        offset =
            -AxisHelper::GetPadding(Padding(), _Axis, false) - m_AlignedSize;
        break;
      case SizeType::CENTER:
        offset = AxisHelper::GetRectSize(Rect(), _Axis) / 2.0f -
                 m_AlignedSize / 2.0f;
        break;
      default:
        break;
      }

      float position = offset +
                       AxisHelper::GetMargin(child->Margin(), _Axis, true) +
                       child->Order() * m_Spacing;

      for (auto &otherChild : Children()) {
        if (otherChild->Order() < child->Order()) {
          position += AxisHelper::GetRectSize(otherChild->Rect(), _Axis) +
                      AxisHelper::GetMargin(otherChild->Margin(), _Axis, true) +
                      AxisHelper::GetMargin(otherChild->Margin(), _Axis, false);
        }
      }

      AxisHelper::GetRectPosition(childRect, _Axis) = position;

      float &pos = AxisHelper::GetRectPosition(childRect, orthogonalAxis);
      float orthoSize = AxisHelper::GetRectSize(childRect, orthogonalAxis);

      switch (AxisHelper::GetLayoutSizeType(size, orthogonalAxis)) {
      case SizeType::FILL:
      case SizeType::START:
        pos = AxisHelper::GetPadding(Padding(), orthogonalAxis, true) +
              AxisHelper::GetMargin(child->Margin(), orthogonalAxis, true);
        break;
      case SizeType::END:
        pos = AxisHelper::GetRectSize(Rect(), orthogonalAxis) -
              AxisHelper::GetPadding(Padding(), orthogonalAxis, false) -
              AxisHelper::GetMargin(child->Margin(), orthogonalAxis, false) -
              orthoSize;
        break;
      case SizeType::CENTER:
        pos = AxisHelper::GetRectSize(Rect(), orthogonalAxis) / 2.0f -
              orthoSize / 2.0f;
        break;
      case SizeType::ABSOLUTE:
        pos = (orthogonalAxis == Axis::HORIZONTAL) ? size.x : size.y;
        break;
      }

      child->SetRect(childRect);
    }
  }

  StackType m_Type;
  ContentType m_ContentType;
  float m_Spacing = 0.0f;
};
} // namespace turnip
