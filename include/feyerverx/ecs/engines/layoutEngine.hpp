// Copyright 2025 Valentin Namleev

#pragma once

#include "feyerverx/axis.hpp"
#include "feyerverx/ecs/components/childrenComponent.hpp"
#include "feyerverx/ecs/components/layoutComponent.hpp"
#include "feyerverx/ecs/components/stackComponent.hpp"
#include "feyerverx/ecs/components/textComponent.hpp"
#include "feyerverx/ecs/components/transformComponent.hpp"
#include "feyerverx/ecs/registry.hpp"

namespace feyerverx::ecs {
class LayoutEngine {
public:
    bool tryMeasureEntityContent(Entity entity);
    bool tryArrangeEntityContent(Entity entity);

private:
    float getRealSize(float value, const Size &size, Axis axis, float maxAvailableValue);

    void measureText(TextComponent *textComponent, TransformComponent *transformComponent);

    void measureNodeContent(ChildrenComponent *childrenComponent,
                            TransformComponent *transformComponent,
                            LayoutComponent *layoutComponent);

    void arrangeNodeContent(ChildrenComponent *childrenComponent,
                            TransformComponent *transformComponent,
                            LayoutComponent *layoutComponent);

    void measureStackContent(Entity entity, ChildrenComponent *childrenComponent,
                             TransformComponent *transformComponent,
                             LayoutComponent *layoutComponent, StackComponent *stackComponent);

    void arrangeStackContent(Entity entity, ChildrenComponent *childrenComponent,
                             TransformComponent *transformComponent,
                             LayoutComponent *layoutComponent, StackComponent *stackComponent);

    std::unordered_map<EntityID, float> m_alignedContentSizes;
};
} // namespace feyerverx::ecs
