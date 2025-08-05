// Copyright 2025 Valentin Namleev

#pragma once

#include <Rectangle.hpp>

#include "./anode.hpp"
#include "./axis.hpp"
#include "./lrtb.hpp"
#include "./rectangleUtils.hpp"
#include "./size.hpp"

namespace turnip {

class Node : public ANode<Node> {
public:
    Node(const std::string &_ID, Size _Size, Color _Color)
        : ANode(_ID), m_Size(_Size), m_Color(_Color) {}

    ~Node() override = default;

    static std::shared_ptr<Node>
    Create(const std::string &_ID, const std::shared_ptr<Node> &_Parent,
           turnip::Size _Size = turnip::Size{turnip::SizeType::FILL, turnip::SizeType::FILL},
           Color _Color = {255, 255, 255, 255}) {
        auto node = std::make_shared<Node>(_ID, _Size, _Color);
        node->SetParent(_Parent);
        return node;
    }

    Rectangle &Rect() { return m_Rect; }
    const Size &Size() const { return m_Size; }
    const LRTB &Margin() const { return m_Margin; }
    const LRTB &Padding() const { return m_Padding; }
    const Rectangle &WorldRect() const { return m_WorldRect; }
    const Color &Color() const { return m_Color; }

    void SetRect(Rectangle _Rect) { m_Rect = _Rect; }
    const void SetPadding(LRTB _Padding) { m_Padding = _Padding; }
    const void SetMargin(LRTB _Margin) { m_Margin = _Margin; }

    virtual void Render() {}
    virtual void RenderDebug() {
        DrawRectangleLinesEx(m_WorldRect, 1.0f, ::RED);
        // DrawRectangleLinesEx(RectangleUtils::Expand(m_WorldRect, m_Margin), 1.0f,
        //                      ::BLUE);
    }

    virtual void Measure() {
        for (auto const &child : Children()) {
            turnip::Size size = child->Size();
            Rectangle childRect = child->Rect();

            for (Axis axis : {Axis::HORIZONTAL, Axis::VERTICAL}) {
                float availableSpace = AxisHelper::GetRectSize(Rect(), axis) -
                                       AxisHelper::GetPadding(Padding(), axis, true) -
                                       AxisHelper::GetPadding(Padding(), axis, false) -
                                       AxisHelper::GetPadding(child->Margin(), axis, true) -
                                       AxisHelper::GetPadding(child->Margin(), axis, false);

                switch (AxisHelper::GetLayoutSizeType(Size(), axis)) {
                case turnip::SizeType::FILL:
                    AxisHelper::GetRectSize(childRect, axis) = availableSpace;
                    break;
                case turnip::SizeType::START:
                case turnip::SizeType::END:
                case turnip::SizeType::CENTER:
                case turnip::SizeType::ABSOLUTE:
                    AxisHelper::GetRectSize(childRect, axis) =
                        AxisHelper::GetLayoutSize(size, axis);
                    break;
                }
            }

            child->SetRect(childRect);
        }
    }

    virtual void Arrange() {
        for (auto &child : Children()) {
            turnip::Size size = child->Size();
            Rectangle childRect = child->Rect();

            for (Axis axis : {Axis::HORIZONTAL, Axis::VERTICAL}) {
                float &pos = AxisHelper::GetRectPosition(childRect, axis);

                switch (AxisHelper::GetLayoutSizeType(size, axis)) {
                case turnip::SizeType::FILL:
                case turnip::SizeType::START:
                    pos = AxisHelper::GetPadding(Padding(), axis, true) +
                          AxisHelper::GetMargin(child->Margin(), axis, true);
                    break;

                case turnip::SizeType::END:
                    pos = AxisHelper::GetRectSize(Rect(), axis) -
                          AxisHelper::GetPadding(Padding(), axis, false) -
                          AxisHelper::GetMargin(child->Margin(), axis, false) -
                          AxisHelper::GetRectSize(childRect, axis);
                    break;

                case turnip::SizeType::CENTER:
                    pos = AxisHelper::GetRectPosition(Rect(), axis) +
                          AxisHelper::GetRectSize(Rect(), axis) / 2.0f -
                          AxisHelper::GetRectSize(childRect, axis) / 2.0f;
                    break;

                case turnip::SizeType::ABSOLUTE:
                    pos = (axis == Axis::HORIZONTAL) ? size.x : size.y;
                    break;
                }
            }

            child->SetRect(childRect);
        }
    }

    void ProcessLayout() {
        Measure();
        Arrange();
        PlaceInWorld();
    }

private:
    void PlaceInWorld() {
        if (auto p = Parent())
            m_WorldRect = RectangleUtils::Move(m_Rect, Vector2{p->WorldRect().x, p->WorldRect().y});
        else
            m_WorldRect = m_Rect;
    }

    turnip::Size m_Size;
    turnip::LRTB m_Padding = {0, 0, 0, 0};
    turnip::LRTB m_Margin = {0, 0, 0, 0};

    Rectangle m_Rect;
    Rectangle m_WorldRect;

    ::Color m_Color;
};
} // namespace turnip
