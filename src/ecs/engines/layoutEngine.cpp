// Copyright 2025 Valentin Namleev

#include "feyerverx/ecs/engines/layoutEngine.hpp"
#include "feyerverx/ecs/components/textComponent.hpp"
#include "feyerverx/ecs/entity.hpp"

#include <algorithm>
#include <iostream>

namespace feyerverx::ecs {
// TODO: Add max available size everywhere, not only to stack contents && test layouting
// TODO: Check jittering on stack height increase
// TODO: Make TransformComponent.worldRect a normal rect, and move rectlerp logic to
// renderTransformComponent
bool LayoutEngine::tryMeasureEntityContent(const Entity entity) {
    ChildrenComponent *childrenComponent = entity.getComponent<ChildrenComponent>();
    TransformComponent *transformComponent = entity.getComponent<TransformComponent>();

    measureText(entity.getComponent<TextComponent>(), transformComponent);

    if (!childrenComponent)
        return false;

    LayoutComponent *layoutComponent = entity.getComponent<LayoutComponent>();

    if (StackComponent *stackComponent = entity.getComponent<StackComponent>(); !stackComponent)
        measureNodeContent(childrenComponent, transformComponent, layoutComponent);
    else
        measureStackContent(entity, childrenComponent, transformComponent, layoutComponent,
                            stackComponent);
    return true;
}

void LayoutEngine::measureText(TextComponent *textComponent,
                               TransformComponent *transformComponent) {
    // TODO: Make this better!
    if (!textComponent)
        return;

    // Vector2f panelSize = _TransformComponent->worldRect.GetSize();
    // float fontSize = _TextComponent->fontSize;
    // Vector2f textSize =
    //     _TextComponent->font.MeasureText(_TextComponent->text, fontSize,
    //     _TextComponent->spacing);

    // if (textSize.x > panelSize.x || textSize.y > panelSize.y) {
    //     while (textSize.x > panelSize.x || textSize.y > panelSize.y) {
    //         fontSize -= 0.1f;
    //         textSize = _TextComponent->font.MeasureText(_TextComponent->text, fontSize,
    //                                                     _TextComponent->spacing);
    //     }
    // } else if (textSize.x < panelSize.x && textSize.y < panelSize.y) {
    //     while (textSize.x < panelSize.x && textSize.y < panelSize.y) {
    //         fontSize += 0.1f;
    //         textSize = _TextComponent->font.MeasureText(_TextComponent->text, fontSize,
    //                                                     _TextComponent->spacing);
    //     }
    // }

    // _TextComponent->fontSize = std::clamp(fontSize, fontSize, _TextComponent->fontSizeOriginal);
}

bool LayoutEngine::tryArrangeEntityContent(Entity entity) {
    ChildrenComponent *childrenComponent = entity.getComponent<ChildrenComponent>();

    if (!childrenComponent)
        return false;

    TransformComponent *transformComponent = entity.getComponent<TransformComponent>();

    LayoutComponent *layoutComponent = entity.getComponent<LayoutComponent>();

    if (StackComponent *stackComponent = entity.getComponent<StackComponent>(); !stackComponent)
        arrangeNodeContent(childrenComponent, transformComponent, layoutComponent);
    else
        arrangeStackContent(entity, childrenComponent, transformComponent, layoutComponent,
                            stackComponent);
    return true;
}

// TODO: Make this BETTER
float LayoutEngine::getRealSize(const float value, const Size &size, const Axis axis,
                                const float maxAvailableValue) {

    const std::optional<float> minSize = AxisHelper::getLayoutConstraintSize(size, axis, true);
    const std::optional<float> maxSize = AxisHelper::getLayoutConstraintSize(size, axis, false);

    const float minValue = minSize.has_value() ? minSize.value() : 0;
    const float maxValue =
        std::min(maxSize.has_value() ? maxSize.value() : std::numeric_limits<float>::max(),
                 maxAvailableValue);

    return std::clamp(value, minValue, maxValue);
}

void LayoutEngine::measureNodeContent(ChildrenComponent *childrenComponent,
                                      TransformComponent *transformComponent,
                                      LayoutComponent *layoutComponent) {
    if (!childrenComponent->children.has_value())
        return;

    for (const auto &child : childrenComponent->children.value()) {
        LayoutComponent *childLayoutComponent = child.getComponent<LayoutComponent>();
        TransformComponent *childTransformComponent = child.getComponent<TransformComponent>();

        for (const Axis axis : {Axis::VERTICAL, Axis::HORIZONTAL}) {
            const float availableSpace =
                AxisHelper::getRectSize(transformComponent->rect, axis) -
                AxisHelper::getPadding(layoutComponent->padding, axis, true) -
                AxisHelper::getPadding(layoutComponent->padding, axis, false) -
                AxisHelper::getPadding(childLayoutComponent->margin, axis, true) -
                AxisHelper::getPadding(childLayoutComponent->margin, axis, false);

            switch (AxisHelper::getLayoutSizeType(childLayoutComponent->size, axis)) {
            case SizeType::FILL:
                AxisHelper::getRectSize(childTransformComponent->rect, axis) = availableSpace;
                break;
            default:
                AxisHelper::getRectSize(childTransformComponent->rect, axis) =
                    getRealSize(AxisHelper::getLayoutSize(childLayoutComponent->size, axis),
                                childLayoutComponent->size, axis,
                                AxisHelper::getLayoutSize(childLayoutComponent->size, axis));
                break;
            }
        }
    }
}

void LayoutEngine::arrangeNodeContent(ChildrenComponent *childrenComponent,
                                      TransformComponent *transformComponent,
                                      LayoutComponent *layoutComponent) {
    if (!childrenComponent->children.has_value())
        return;

    for (const auto &child : childrenComponent->children.value()) {
        LayoutComponent *childLayoutComponent = child.getComponent<LayoutComponent>();
        TransformComponent *childTransformComponent = child.getComponent<TransformComponent>();

        for (const Axis axis : {Axis::HORIZONTAL, Axis::VERTICAL}) {
            float &pos = AxisHelper::getRectPosition(childTransformComponent->rect, axis);

            switch (AxisHelper::getLayoutSizeType(childLayoutComponent->size, axis)) {
            case SizeType::FILL:
            case SizeType::START:
                pos = AxisHelper::getPadding(layoutComponent->padding, axis, true) +
                      AxisHelper::getMargin(childLayoutComponent->margin, axis, true);
                break;
            case SizeType::END:
                pos = AxisHelper::getRectSize(transformComponent->rect, axis) -
                      AxisHelper::getPadding(layoutComponent->padding, axis, false) -
                      AxisHelper::getMargin(childLayoutComponent->margin, axis, false) -
                      AxisHelper::getRectSize(childTransformComponent->rect, axis);
                break;
            case SizeType::CENTER:
                pos = AxisHelper::getRectPosition(transformComponent->rect, axis) +
                      AxisHelper::getRectSize(transformComponent->rect, axis) * 0.5f -
                      AxisHelper::getRectSize(childTransformComponent->rect, axis) * 0.5f;
                break;
            case SizeType::ABSOLUTE:
                pos = (axis == Axis::HORIZONTAL) ? childLayoutComponent->size.x
                                                 : childLayoutComponent->size.y;
                break;
            }
        }
    }
}

void LayoutEngine::measureStackContent(Entity entity, ChildrenComponent *childrenComponent,
                                       TransformComponent *transformComponent,
                                       LayoutComponent *layoutComponent,
                                       StackComponent *stackComponent) {
    if (!childrenComponent->children.has_value())
        return;

    Axis axis = stackComponent->type == StackType::HORIZONTAL ? Axis::HORIZONTAL : Axis::VERTICAL;

    float fillSpace = AxisHelper::getRectSize(transformComponent->rect, axis) -
                      AxisHelper::getPadding(layoutComponent->padding, axis, true) -
                      AxisHelper::getPadding(layoutComponent->padding, axis, false) -
                      (childrenComponent->children.value().size() - 1) * stackComponent->spacing;
    float virtualFillSpace = fillSpace;

    int fillChildrenCount = 0;

    for (auto const &child : childrenComponent->children.value()) {
        LayoutComponent *childLayoutComponent = child.getComponent<LayoutComponent>();

        fillSpace -= AxisHelper::getMargin(childLayoutComponent->margin, axis, true) +
                     AxisHelper::getMargin(childLayoutComponent->margin, axis, false);

        if (AxisHelper::getLayoutSizeType(childLayoutComponent->size, axis) != SizeType::FILL) {
            fillSpace -= AxisHelper::getLayoutSize(childLayoutComponent->size, axis);
        } else
            fillChildrenCount += 1;
    }

    const Axis orthogonalAxis = (axis == Axis::HORIZONTAL) ? Axis::VERTICAL : Axis::HORIZONTAL;

    m_alignedContentSizes[entity.ID()] =
        (childrenComponent->children.value().size() - 1) * stackComponent->spacing;

    for (auto const &child : childrenComponent->children.value()) {
        TransformComponent *childTransformComponent = child.getComponent<TransformComponent>();

        LayoutComponent *childLayoutComponent = child.getComponent<LayoutComponent>();

        switch (AxisHelper::getLayoutSizeType(childLayoutComponent->size, axis)) {
        case SizeType::FILL:
            m_alignedContentSizes[entity.ID()] +=
                AxisHelper::getRectSize(childTransformComponent->rect, axis) =
                    getRealSize(fillSpace / fillChildrenCount, childLayoutComponent->size, axis,
                                fillSpace / fillChildrenCount);

            break;
        default:
            m_alignedContentSizes[entity.ID()] +=
                AxisHelper::getRectSize(childTransformComponent->rect, axis) =
                    getRealSize(AxisHelper::getLayoutSize(childLayoutComponent->size, axis),
                                childLayoutComponent->size, axis,
                                virtualFillSpace / childrenComponent->children.value().size());
            break;
        }

        float orthoAvailable =
            AxisHelper::getRectSize(transformComponent->rect, orthogonalAxis) -
            AxisHelper::getPadding(layoutComponent->padding, orthogonalAxis, true) -
            AxisHelper::getPadding(layoutComponent->padding, orthogonalAxis, false) -
            AxisHelper::getMargin(childLayoutComponent->margin, orthogonalAxis, true) -
            AxisHelper::getMargin(childLayoutComponent->margin, orthogonalAxis, false);

        switch (AxisHelper::getLayoutSizeType(childLayoutComponent->size, orthogonalAxis)) {
        case SizeType::FILL:
            AxisHelper::getRectSize(childTransformComponent->rect, orthogonalAxis) = orthoAvailable;
            break;
        default:
            AxisHelper::getRectSize(childTransformComponent->rect, orthogonalAxis) =
                AxisHelper::getLayoutSize(childLayoutComponent->size, orthogonalAxis);
            break;
        }
    }
}

void LayoutEngine::arrangeStackContent(Entity entity, ChildrenComponent *childrenComponent,
                                       TransformComponent *transformComponent,
                                       LayoutComponent *layoutComponent,
                                       StackComponent *stackComponent) {
    if (!childrenComponent->children.has_value())
        return;

    const Axis axis =
        (stackComponent->type == StackType::HORIZONTAL) ? Axis::HORIZONTAL : Axis::VERTICAL;
    const Axis orthogonalAxis = (axis == Axis::HORIZONTAL) ? Axis::VERTICAL : Axis::HORIZONTAL;

    float currentOffset = 0;
    switch (stackComponent->contentType) {
    case StackContentType::START:
        currentOffset = AxisHelper::getPadding(layoutComponent->padding, axis, true);
        break;
    case StackContentType::CENTER:
        currentOffset = AxisHelper::getRectSize(transformComponent->rect, axis) / 2 -
                        m_alignedContentSizes[entity.ID()] / 2;
        break;
    case StackContentType::END:
        currentOffset = AxisHelper::getRectSize(transformComponent->rect, axis) -
                        AxisHelper::getPadding(layoutComponent->padding, axis, false) -
                        m_alignedContentSizes[entity.ID()];
        break;
    }

    for (size_t i = 0; i < childrenComponent->children.value().size(); ++i) {
        Entity child = childrenComponent->children.value()[i];
        TransformComponent *childTransform = child.getComponent<TransformComponent>();
        LayoutComponent *childLayout = child.getComponent<LayoutComponent>();

        // main axis
        currentOffset += AxisHelper::getMargin(childLayout->margin, axis, true);

        AxisHelper::getRectPosition(childTransform->rect, axis) = currentOffset;

        currentOffset += AxisHelper::getRectSize(childTransform->rect, axis);
        currentOffset += AxisHelper::getMargin(childLayout->margin, axis, false);
        currentOffset += stackComponent->spacing;

        // orthogonal axis
        float &pos = AxisHelper::getRectPosition(childTransform->rect, orthogonalAxis);
        float size = AxisHelper::getRectSize(childTransform->rect, orthogonalAxis);

        switch (AxisHelper::getLayoutSizeType(childLayout->size, orthogonalAxis)) {
        case SizeType::FILL:
        case SizeType::START:
            pos = AxisHelper::getPadding(layoutComponent->padding, orthogonalAxis, true) +
                  AxisHelper::getMargin(childLayout->margin, orthogonalAxis, true);
            break;
        case SizeType::END:
            pos = AxisHelper::getRectSize(transformComponent->rect, orthogonalAxis) -
                  AxisHelper::getPadding(layoutComponent->padding, orthogonalAxis, false) -
                  AxisHelper::getMargin(childLayout->margin, orthogonalAxis, false) - size;
            break;
        case SizeType::CENTER:
            pos = AxisHelper::getRectSize(transformComponent->rect, orthogonalAxis) * 0.5f -
                  size * 0.5f;
            break;
        case SizeType::ABSOLUTE:
            pos = (orthogonalAxis == Axis::HORIZONTAL) ? childLayout->size.x : childLayout->size.y;
            break;
        }
    }
}
} // namespace feyerverx::ecs
