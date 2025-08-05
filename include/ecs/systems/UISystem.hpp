// Copyright 2025 Valentin Namleev

#pragma once

#include "../../axis.hpp"
#include "../../rectangleUtils.hpp"
#include "../components/layoutComponent.hpp"
#include "../components/parentComponent.hpp"
#include "../components/stackComponent.hpp"
#include "../components/transformComponent.hpp"
#include "../registry.hpp"
#include "../system.hpp"

#include <unordered_map>

namespace turnip::ecs {
class UISystem : protected ISystem {
public:
    UISystem(Registry &_Registry, Vector2 _Size) : ISystem(_Registry), m_Size(_Size) {}

    void Update(float _DeltaTime) override { ProcessLayout(); }

private:
    Vector2 m_Size;
    std::unordered_map<EntityID, float> m_AlignedSizes;

    void ProcessLayout() {
        auto roots = FindRoots();
        for (auto root : roots) {

            auto transform = m_Registry.GetComponent<TransformComponent>(root);
            transform->rect.width = m_Size.x;
            transform->rect.height = m_Size.y;

            Measure(root);
            Arrange(root);
            PlaceInWorld(root);
        }
    }

    std::vector<EntityID> FindRoots() {
        std::vector<EntityID> roots;

        for (const auto &e : m_Registry.With<TransformComponent>()) {
            if (!m_Registry.GetComponent<ParentComponent>(e)) {
                roots.push_back(e);
            }
        }

        return roots;
    }

    void Measure(EntityID _EntityID) {
        ChildrenComponent *childrenComponent =
            m_Registry.GetComponent<ChildrenComponent>(_EntityID);

        if (!childrenComponent) {
            return;
        }

        TransformComponent *transformComponent =
            m_Registry.GetComponent<TransformComponent>(_EntityID);

        LayoutComponent *layoutComponent = m_Registry.GetComponent<LayoutComponent>(_EntityID);

        if (m_Registry.GetComponent<StackComponent>(_EntityID))
            MeasureAxis(_EntityID, childrenComponent);
        else {
            for (const auto &child : childrenComponent->children) {
                LayoutComponent *childLayoutComponent =
                    m_Registry.GetComponent<LayoutComponent>(child);
                TransformComponent *childTransformComponent =
                    m_Registry.GetComponent<TransformComponent>(child);

                for (Axis axis : {Axis::VERTICAL, Axis::HORIZONTAL}) {
                    float availableSpace =
                        AxisHelper::GetRectSize(transformComponent->rect, axis) -
                        AxisHelper::GetPadding(layoutComponent->padding, axis, true) -
                        AxisHelper::GetPadding(layoutComponent->padding, axis, false) -
                        AxisHelper::GetPadding(childLayoutComponent->margin, axis, true) -
                        AxisHelper::GetPadding(childLayoutComponent->margin, axis, false);

                    switch (AxisHelper::GetLayoutSizeType(childLayoutComponent->size, axis)) {
                    case turnip::SizeType::FILL:
                        AxisHelper::GetRectSize(childTransformComponent->rect, axis) =
                            availableSpace;
                        break;
                    case turnip::SizeType::START:
                    case turnip::SizeType::END:
                    case turnip::SizeType::CENTER:
                    case turnip::SizeType::ABSOLUTE:
                        AxisHelper::GetRectSize(childTransformComponent->rect, axis) =
                            AxisHelper::GetLayoutSize(childLayoutComponent->size, axis);
                        break;
                    }
                }
            }
        }

        for (const auto &child : childrenComponent->children) {
            Measure(child);
        }
    }

    void Arrange(EntityID _EntityID) {
        ChildrenComponent *childrenComponent =
            m_Registry.GetComponent<ChildrenComponent>(_EntityID);

        if (!childrenComponent)
            return;

        LayoutComponent *layoutComponent = m_Registry.GetComponent<LayoutComponent>(_EntityID);
        TransformComponent *transformComponent =
            m_Registry.GetComponent<TransformComponent>(_EntityID);

        if (m_Registry.GetComponent<StackComponent>(_EntityID))
            ArrangeAxis(_EntityID, childrenComponent);
        else {
            for (const auto &child : childrenComponent->children) {
                LayoutComponent *childLayoutComponent =
                    m_Registry.GetComponent<LayoutComponent>(child);
                TransformComponent *childTransformComponent =
                    m_Registry.GetComponent<TransformComponent>(child);

                for (Axis axis : {Axis::HORIZONTAL, Axis::VERTICAL}) {
                    float &pos = AxisHelper::GetRectPosition(childTransformComponent->rect, axis);

                    switch (AxisHelper::GetLayoutSizeType(childLayoutComponent->size, axis)) {
                    case turnip::SizeType::FILL:
                    case turnip::SizeType::START:
                        pos = AxisHelper::GetPadding(layoutComponent->padding, axis, true) +
                              AxisHelper::GetMargin(childLayoutComponent->margin, axis, true);
                        break;
                    case turnip::SizeType::END:
                        pos = AxisHelper::GetRectSize(transformComponent->rect, axis) -
                              AxisHelper::GetPadding(layoutComponent->padding, axis, false) -
                              AxisHelper::GetMargin(childLayoutComponent->margin, axis, false) -
                              AxisHelper::GetRectSize(childTransformComponent->rect, axis);
                        break;
                    case turnip::SizeType::CENTER:
                        pos = AxisHelper::GetRectPosition(transformComponent->rect, axis) +
                              AxisHelper::GetRectSize(transformComponent->rect, axis) * 0.5f -
                              AxisHelper::GetRectSize(childTransformComponent->rect, axis) * 0.5f;
                        break;
                    case turnip::SizeType::ABSOLUTE:
                        pos = (axis == Axis::HORIZONTAL) ? childLayoutComponent->size.x
                                                         : childLayoutComponent->size.y;
                        break;
                    }
                }
            }
        }

        for (const auto &child : childrenComponent->children) {
            Arrange(child);
        }
    }

    void PlaceInWorld(EntityID _EntityID) {
        TransformComponent *transformComponent =
            m_Registry.GetComponent<TransformComponent>(_EntityID);

        ParentComponent *parentComponent = m_Registry.GetComponent<ParentComponent>(_EntityID);

        ChildrenComponent *childrenComponent =
            m_Registry.GetComponent<ChildrenComponent>(_EntityID);

        if (!parentComponent)
            transformComponent->worldRect = transformComponent->rect;
        else {
            TransformComponent *parentTransformComponent =
                m_Registry.GetComponent<TransformComponent>(parentComponent->parent);
            transformComponent->worldRect = RectangleUtils::Move(
                transformComponent->rect, Vector2{parentTransformComponent->worldRect.x,
                                                  parentTransformComponent->worldRect.y});
        }

        if (!childrenComponent)
            return;

        for (const auto &child : childrenComponent->children) {
            PlaceInWorld(child);
        }
    }

    void MeasureAxis(EntityID _EntityID, ChildrenComponent *_ChildrenComponent) {
        TransformComponent *transformComponent =
            m_Registry.GetComponent<TransformComponent>(_EntityID);

        LayoutComponent *layoutComponent = m_Registry.GetComponent<LayoutComponent>(_EntityID);

        StackComponent *stackComponent = m_Registry.GetComponent<StackComponent>(_EntityID);

        Axis axis =
            stackComponent->type == StackType::HORIZONTAL ? Axis::HORIZONTAL : Axis::VERTICAL;

        float fillSpace = AxisHelper::GetRectSize(transformComponent->rect, axis) -
                          AxisHelper::GetPadding(layoutComponent->padding, axis, true) -
                          AxisHelper::GetPadding(layoutComponent->padding, axis, false) -
                          (_ChildrenComponent->children.size() - 1) * stackComponent->spacing;

        int fillChildrenCount = 0;

        for (auto const &child : _ChildrenComponent->children) {
            TransformComponent *childTransformComponent =
                m_Registry.GetComponent<TransformComponent>(child);

            LayoutComponent *childLayoutComponent = m_Registry.GetComponent<LayoutComponent>(child);

            fillSpace -= AxisHelper::GetMargin(childLayoutComponent->margin, axis, true) +
                         AxisHelper::GetMargin(childLayoutComponent->margin, axis, false);

            if (AxisHelper::GetLayoutSizeType(childLayoutComponent->size, axis) != SizeType::FILL) {
                fillSpace -= AxisHelper::GetLayoutSize(childLayoutComponent->size, axis);
            } else
                fillChildrenCount += 1;
        }

        Axis orthogonalAxis = (axis == Axis::HORIZONTAL) ? Axis::VERTICAL : Axis::HORIZONTAL;

        for (auto const &child : _ChildrenComponent->children) {
            m_AlignedSizes[_EntityID] = 0;
            TransformComponent *childTransformComponent =
                m_Registry.GetComponent<TransformComponent>(child);

            LayoutComponent *childLayoutComponent = m_Registry.GetComponent<LayoutComponent>(child);

            switch (AxisHelper::GetLayoutSizeType(childLayoutComponent->size, axis)) {
            case SizeType::FILL:
                m_AlignedSizes[_EntityID] +=
                    AxisHelper::GetRectSize(childTransformComponent->rect, axis) =
                        fillSpace / fillChildrenCount;
                break;
            default:
                m_AlignedSizes[_EntityID] +=
                    AxisHelper::GetRectSize(childTransformComponent->rect, axis) =
                        AxisHelper::GetLayoutSize(childLayoutComponent->size, axis);
                break;
            }

            float orthoAvailable =
                AxisHelper::GetRectSize(transformComponent->rect, orthogonalAxis) -
                AxisHelper::GetPadding(layoutComponent->padding, orthogonalAxis, true) -
                AxisHelper::GetPadding(layoutComponent->padding, orthogonalAxis, false) -
                AxisHelper::GetMargin(childLayoutComponent->margin, orthogonalAxis, true) -
                AxisHelper::GetMargin(childLayoutComponent->margin, orthogonalAxis, false);

            switch (AxisHelper::GetLayoutSizeType(childLayoutComponent->size, orthogonalAxis)) {
            case SizeType::FILL:
                AxisHelper::GetRectSize(childTransformComponent->rect, orthogonalAxis) =
                    orthoAvailable;
                break;
            default:
                AxisHelper::GetRectSize(childTransformComponent->rect, orthogonalAxis) =
                    AxisHelper::GetLayoutSize(childLayoutComponent->size, orthogonalAxis);
                break;
            }
        }
    }

    void ArrangeAxis(EntityID _EntityID, ChildrenComponent *_ChildrenComponent) {
        TransformComponent *transformComponent =
            m_Registry.GetComponent<TransformComponent>(_EntityID);
        LayoutComponent *layoutComponent = m_Registry.GetComponent<LayoutComponent>(_EntityID);
        StackComponent *stackComponent = m_Registry.GetComponent<StackComponent>(_EntityID);

        Axis axis =
            (stackComponent->type == StackType::HORIZONTAL) ? Axis::HORIZONTAL : Axis::VERTICAL;
        Axis orthogonalAxis = (axis == Axis::HORIZONTAL) ? Axis::VERTICAL : Axis::HORIZONTAL;

        float currentOffset = AxisHelper::GetPadding(layoutComponent->padding, axis, true);

        for (size_t i = 0; i < _ChildrenComponent->children.size(); ++i) {
            EntityID child = _ChildrenComponent->children[i];
            TransformComponent *childTransform = m_Registry.GetComponent<TransformComponent>(child);
            LayoutComponent *childLayout = m_Registry.GetComponent<LayoutComponent>(child);

            // main axis
            currentOffset += AxisHelper::GetMargin(childLayout->margin, axis, true);

            AxisHelper::GetRectPosition(childTransform->rect, axis) = currentOffset;

            currentOffset += AxisHelper::GetRectSize(childTransform->rect, axis);
            currentOffset += AxisHelper::GetMargin(childLayout->margin, axis, false);
            currentOffset += stackComponent->spacing;

            // orthogonal axis
            float &pos = AxisHelper::GetRectPosition(childTransform->rect, orthogonalAxis);
            float size = AxisHelper::GetRectSize(childTransform->rect, orthogonalAxis);

            switch (AxisHelper::GetLayoutSizeType(childLayout->size, orthogonalAxis)) {
            case SizeType::FILL:
            case SizeType::START:
                pos = AxisHelper::GetPadding(layoutComponent->padding, orthogonalAxis, true) +
                      AxisHelper::GetMargin(childLayout->margin, orthogonalAxis, true);
                break;
            case SizeType::END:
                pos = AxisHelper::GetRectSize(transformComponent->rect, orthogonalAxis) -
                      AxisHelper::GetPadding(layoutComponent->padding, orthogonalAxis, false) -
                      AxisHelper::GetMargin(childLayout->margin, orthogonalAxis, false) - size;
                break;
            case SizeType::CENTER:
                pos = AxisHelper::GetRectSize(transformComponent->rect, orthogonalAxis) * 0.5f -
                      size * 0.5f;
                break;
            case SizeType::ABSOLUTE:
                pos = (orthogonalAxis == Axis::HORIZONTAL) ? childLayout->size.x
                                                           : childLayout->size.y;
                break;
            }
        }
    }

    int GetChildOrder(EntityID _Child, std::vector<EntityID> _Children) {
        auto it = std::find_if(_Children.begin(), _Children.end(),
                               [_Child](EntityID _EntityID) { return _EntityID == _Child; });

        return (it != _Children.end()) ? static_cast<int>(std::distance(_Children.begin(), it)) : 0;
    }
};
} // namespace turnip::ecs
