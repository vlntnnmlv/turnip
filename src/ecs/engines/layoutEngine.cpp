// Copyright 2025 Valentin Namleev

#include "./ecs/engines/layoutEngine.hpp"

namespace turnip::ecs {
LayoutEngine::LayoutEngine(Registry &_Registry) : m_Registry(_Registry) {}

bool LayoutEngine::TryMeasureEntityContent(EntityID _EntityID) {
    ChildrenComponent *childrenComponent = m_Registry.GetComponent<ChildrenComponent>(_EntityID);

    if (!childrenComponent)
        return false;

    TransformComponent *transformComponent = m_Registry.GetComponent<TransformComponent>(_EntityID);

    LayoutComponent *layoutComponent = m_Registry.GetComponent<LayoutComponent>(_EntityID);
    StackComponent *stackComponent = m_Registry.GetComponent<StackComponent>(_EntityID);

    if (!stackComponent)
        MeasureNodeContent(_EntityID, childrenComponent, transformComponent, layoutComponent);
    else
        MeasureStackContent(_EntityID, childrenComponent, transformComponent, layoutComponent,
                            stackComponent);

    return true;
}

bool LayoutEngine::TryArrangeEntityContent(EntityID _EntityID) {
    ChildrenComponent *childrenComponent = m_Registry.GetComponent<ChildrenComponent>(_EntityID);

    if (!childrenComponent)
        return false;

    TransformComponent *transformComponent = m_Registry.GetComponent<TransformComponent>(_EntityID);

    LayoutComponent *layoutComponent = m_Registry.GetComponent<LayoutComponent>(_EntityID);
    StackComponent *stackComponent = m_Registry.GetComponent<StackComponent>(_EntityID);

    if (!stackComponent)
        ArrangeNodeContent(_EntityID, childrenComponent, transformComponent, layoutComponent);
    else
        ArrangeStackContent(_EntityID, childrenComponent, transformComponent, layoutComponent,
                            stackComponent);

    return true;
}

float LayoutEngine::GetRealSize(Size _Size, Axis _Axis) {
    std::optional<float> minSize = AxisHelper::GetLayoutConstraintSize(_Size, _Axis, true);

    std::optional<float> maxSize = AxisHelper::GetLayoutConstraintSize(_Size, _Axis, false);

    float size = std::clamp(
        AxisHelper::GetLayoutSize(_Size, _Axis), minSize.has_value() ? minSize.value() : 0,
        maxSize.has_value() ? maxSize.value() : std::numeric_limits<float>::max());

    return size;
}

void LayoutEngine::MeasureNodeContent(EntityID _EntityID, ChildrenComponent *_ChildrenComponent,
                                      TransformComponent *_TransformComponent,
                                      LayoutComponent *_LayoutComponent) {
    for (const auto &child : _ChildrenComponent->children) {
        LayoutComponent *childLayoutComponent = m_Registry.GetComponent<LayoutComponent>(child);
        TransformComponent *childTransformComponent =
            m_Registry.GetComponent<TransformComponent>(child);

        for (Axis axis : {Axis::VERTICAL, Axis::HORIZONTAL}) {
            float availableSpace =
                AxisHelper::GetRectSize(_TransformComponent->rect, axis) -
                AxisHelper::GetPadding(_LayoutComponent->padding, axis, true) -
                AxisHelper::GetPadding(_LayoutComponent->padding, axis, false) -
                AxisHelper::GetPadding(childLayoutComponent->margin, axis, true) -
                AxisHelper::GetPadding(childLayoutComponent->margin, axis, false);

            switch (AxisHelper::GetLayoutSizeType(childLayoutComponent->size, axis)) {
            case turnip::SizeType::FILL:
                AxisHelper::GetRectSize(childTransformComponent->rect, axis) = availableSpace;
                break;
            default:
                AxisHelper::GetRectSize(childTransformComponent->rect, axis) =
                    GetRealSize(childLayoutComponent->size, axis);
                break;
            }
        }
    }
}

void LayoutEngine::ArrangeNodeContent(EntityID _EntityID, ChildrenComponent *_ChildrenComponent,
                                      TransformComponent *_TransformComponent,
                                      LayoutComponent *_LayoutComponent) {
    for (const auto &child : _ChildrenComponent->children) {
        LayoutComponent *childLayoutComponent = m_Registry.GetComponent<LayoutComponent>(child);
        TransformComponent *childTransformComponent =
            m_Registry.GetComponent<TransformComponent>(child);

        for (Axis axis : {Axis::HORIZONTAL, Axis::VERTICAL}) {
            float &pos = AxisHelper::GetRectPosition(childTransformComponent->rect, axis);

            switch (AxisHelper::GetLayoutSizeType(childLayoutComponent->size, axis)) {
            case turnip::SizeType::FILL:
            case turnip::SizeType::START:
                pos = AxisHelper::GetPadding(_LayoutComponent->padding, axis, true) +
                      AxisHelper::GetMargin(childLayoutComponent->margin, axis, true);
                break;
            case turnip::SizeType::END:
                pos = AxisHelper::GetRectSize(_TransformComponent->rect, axis) -
                      AxisHelper::GetPadding(_LayoutComponent->padding, axis, false) -
                      AxisHelper::GetMargin(childLayoutComponent->margin, axis, false) -
                      AxisHelper::GetRectSize(childTransformComponent->rect, axis);
                break;
            case turnip::SizeType::CENTER:
                pos = AxisHelper::GetRectPosition(_TransformComponent->rect, axis) +
                      AxisHelper::GetRectSize(_TransformComponent->rect, axis) * 0.5f -
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

void LayoutEngine::MeasureStackContent(EntityID _EntityID, ChildrenComponent *_ChildrenComponent,
                                       TransformComponent *_TransformComponent,
                                       LayoutComponent *_LayoutComponent,
                                       StackComponent *_StackComponent) {
    Axis axis = _StackComponent->type == StackType::HORIZONTAL ? Axis::HORIZONTAL : Axis::VERTICAL;

    float fillSpace = AxisHelper::GetRectSize(_TransformComponent->rect, axis) -
                      AxisHelper::GetPadding(_LayoutComponent->padding, axis, true) -
                      AxisHelper::GetPadding(_LayoutComponent->padding, axis, false) -
                      (_ChildrenComponent->children.size() - 1) * _StackComponent->spacing;

    int fillChildrenCount = 0;

    for (auto const &child : _ChildrenComponent->children) {
        LayoutComponent *childLayoutComponent = m_Registry.GetComponent<LayoutComponent>(child);

        fillSpace -= AxisHelper::GetMargin(childLayoutComponent->margin, axis, true) +
                     AxisHelper::GetMargin(childLayoutComponent->margin, axis, false);

        if (AxisHelper::GetLayoutSizeType(childLayoutComponent->size, axis) != SizeType::FILL) {
            fillSpace -= AxisHelper::GetLayoutSize(childLayoutComponent->size, axis);
        } else
            fillChildrenCount += 1;
    }

    Axis orthogonalAxis = (axis == Axis::HORIZONTAL) ? Axis::VERTICAL : Axis::HORIZONTAL;

    m_AlignedContentSizes[_EntityID] =
        (_ChildrenComponent->children.size() - 1) * _StackComponent->spacing;

    for (auto const &child : _ChildrenComponent->children) {
        TransformComponent *childTransformComponent =
            m_Registry.GetComponent<TransformComponent>(child);

        LayoutComponent *childLayoutComponent = m_Registry.GetComponent<LayoutComponent>(child);

        switch (AxisHelper::GetLayoutSizeType(childLayoutComponent->size, axis)) {
        case SizeType::FILL:
            m_AlignedContentSizes[_EntityID] +=
                AxisHelper::GetRectSize(childTransformComponent->rect, axis) =
                    fillSpace / fillChildrenCount;
            break;
        default:
            m_AlignedContentSizes[_EntityID] +=
                AxisHelper::GetRectSize(childTransformComponent->rect, axis) =
                    GetRealSize(childLayoutComponent->size, axis);
            break;
        }

        float orthoAvailable =
            AxisHelper::GetRectSize(_TransformComponent->rect, orthogonalAxis) -
            AxisHelper::GetPadding(_LayoutComponent->padding, orthogonalAxis, true) -
            AxisHelper::GetPadding(_LayoutComponent->padding, orthogonalAxis, false) -
            AxisHelper::GetMargin(childLayoutComponent->margin, orthogonalAxis, true) -
            AxisHelper::GetMargin(childLayoutComponent->margin, orthogonalAxis, false);

        switch (AxisHelper::GetLayoutSizeType(childLayoutComponent->size, orthogonalAxis)) {
        case SizeType::FILL:
            AxisHelper::GetRectSize(childTransformComponent->rect, orthogonalAxis) = orthoAvailable;
            break;
        default:
            AxisHelper::GetRectSize(childTransformComponent->rect, orthogonalAxis) =
                AxisHelper::GetLayoutSize(childLayoutComponent->size, orthogonalAxis);
            break;
        }
    }
}

void LayoutEngine::ArrangeStackContent(EntityID _EntityID, ChildrenComponent *_ChildrenComponent,
                                       TransformComponent *_TransformComponent,
                                       LayoutComponent *_LayoutComponent,
                                       StackComponent *_StackComponent) {
    Axis axis =
        (_StackComponent->type == StackType::HORIZONTAL) ? Axis::HORIZONTAL : Axis::VERTICAL;
    Axis orthogonalAxis = (axis == Axis::HORIZONTAL) ? Axis::VERTICAL : Axis::HORIZONTAL;

    float currentOffset;
    switch (_StackComponent->contentType) {
    case StackContentType::START:
        currentOffset = AxisHelper::GetPadding(_LayoutComponent->padding, axis, true);
        break;
    case StackContentType::CENTER:
        currentOffset = AxisHelper::GetRectSize(_TransformComponent->rect, axis) / 2 -
                        m_AlignedContentSizes[_EntityID] / 2;
        break;
    case StackContentType::END:
        currentOffset = AxisHelper::GetRectSize(_TransformComponent->rect, axis) -
                        AxisHelper::GetPadding(_LayoutComponent->padding, axis, false) -
                        m_AlignedContentSizes[_EntityID];
        break;
    }

    for (size_t i = 0; i < _ChildrenComponent->children.size(); ++i) {
        EntityID child = _ChildrenComponent->children[i];
        TransformComponent *childTransform = m_Registry.GetComponent<TransformComponent>(child);
        LayoutComponent *childLayout = m_Registry.GetComponent<LayoutComponent>(child);

        // main axis
        currentOffset += AxisHelper::GetMargin(childLayout->margin, axis, true);

        AxisHelper::GetRectPosition(childTransform->rect, axis) = currentOffset;

        currentOffset += AxisHelper::GetRectSize(childTransform->rect, axis);
        currentOffset += AxisHelper::GetMargin(childLayout->margin, axis, false);
        currentOffset += _StackComponent->spacing;

        // orthogonal axis
        float &pos = AxisHelper::GetRectPosition(childTransform->rect, orthogonalAxis);
        float size = AxisHelper::GetRectSize(childTransform->rect, orthogonalAxis);

        switch (AxisHelper::GetLayoutSizeType(childLayout->size, orthogonalAxis)) {
        case SizeType::FILL:
        case SizeType::START:
            pos = AxisHelper::GetPadding(_LayoutComponent->padding, orthogonalAxis, true) +
                  AxisHelper::GetMargin(childLayout->margin, orthogonalAxis, true);
            break;
        case SizeType::END:
            pos = AxisHelper::GetRectSize(_TransformComponent->rect, orthogonalAxis) -
                  AxisHelper::GetPadding(_LayoutComponent->padding, orthogonalAxis, false) -
                  AxisHelper::GetMargin(childLayout->margin, orthogonalAxis, false) - size;
            break;
        case SizeType::CENTER:
            pos = AxisHelper::GetRectSize(_TransformComponent->rect, orthogonalAxis) * 0.5f -
                  size * 0.5f;
            break;
        case SizeType::ABSOLUTE:
            pos = (orthogonalAxis == Axis::HORIZONTAL) ? childLayout->size.x : childLayout->size.y;
            break;
        }
    }
}
} // namespace turnip::ecs
