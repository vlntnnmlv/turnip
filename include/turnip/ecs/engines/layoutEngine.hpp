// Copyright 2025 Valentin Namleev

#pragma once

#include <algorithm>
#include <limits>

#include "./turnip/axis.hpp"
#include "./turnip/ecs/components/childrenComponent.hpp"
#include "./turnip/ecs/components/layoutComponent.hpp"
#include "./turnip/ecs/components/stackComponent.hpp"
#include "./turnip/ecs/components/transformComponent.hpp"
#include "./turnip/ecs/registry.hpp"

namespace turnip::ecs {
class LayoutEngine {
public:
    LayoutEngine(Registry &_Registry);

    bool TryMeasureEntityContent(EntityID _EntityID);
    bool TryArrangeEntityContent(EntityID _EntityID);

private:
    float GetRealSize(float _Value, Size _Size, Axis _Axis, float _MaxAvailableValue);

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
    Registry &m_Registry;
};
} // namespace turnip::ecs
