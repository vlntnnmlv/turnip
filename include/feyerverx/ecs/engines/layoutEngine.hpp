// Copyright 2025 Valentin Namleev

#pragma once

#include <algorithm>
#include <limits>

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
    LayoutEngine(Registry &_Registry);

    bool TryMeasureEntityContent(EntityID _EntityID);
    bool TryArrangeEntityContent(EntityID _EntityID);

private:
    float GetRealSize(float _Value, Size _Size, Axis _Axis, float _MaxAvailableValue);

    void MeasureText(TextComponent *_TextComponent, TransformComponent *_TransformComponent);

    void MeasureNodeContent(ChildrenComponent *_ChildrenComponent,
                            TransformComponent *_TransformComponent,
                            LayoutComponent *_LayoutComponent);

    void ArrangeNodeContent(ChildrenComponent *_ChildrenComponent,
                            TransformComponent *_TransformComponent,
                            LayoutComponent *_LayoutComponent);

    void MeasureStackContent(EntityID _EntityID, ChildrenComponent *_ChildrenComponent,
                             TransformComponent *_TransformComponent,
                             LayoutComponent *_LayoutComponent, StackComponent *_StackComponent);

    void ArrangeStackContent(EntityID _EntityID, ChildrenComponent *_ChildrenComponent,
                             TransformComponent *_TransformComponent,
                             LayoutComponent *_LayoutComponent, StackComponent *_StackComponent);

    std::unordered_map<EntityID, float> m_AlignedContentSizes;
    Registry &m_registry;
};
} // namespace feyerverx::ecs
