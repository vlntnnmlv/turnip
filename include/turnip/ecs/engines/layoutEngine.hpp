// Copyright 2025 Valentin Namleev

#pragma once

#include <algorithm>
#include <limits>

#include "../../axis.hpp"
#include "../components/childrenComponent.hpp"
#include "../components/layoutComponent.hpp"
#include "../components/stackComponent.hpp"
#include "../components/transformComponent.hpp"
#include "../registry.hpp"

namespace turnip::ecs {
class LayoutEngine {
public:
    LayoutEngine(Registry &_Registry);

    bool TryMeasureEntityContent(EntityID _EntityID);
    bool TryArrangeEntityContent(EntityID _EntityID);

private:
    float GetRealSize(Size _Size, Axis _Axis);

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
