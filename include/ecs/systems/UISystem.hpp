// Copyright 2025 Valentin Namleev

#pragma once

#include "../../axis.hpp"
#include "../../rectangleUtils.hpp"
#include "../components/layoutComponent.hpp"
#include "../components/parentComponent.hpp"
#include "../components/stackComponent.hpp"
#include "../components/transformComponent.hpp"
#include "../engines/layoutEngine.hpp"
#include "../registry.hpp"
#include "../system.hpp"

#include <unordered_map>

namespace turnip::ecs {
class UISystem : protected ISystem {
public:
    UISystem(Registry &_Registry, Vector2 _Size)
        : ISystem(_Registry), m_LayoutEngine(m_Registry), m_Size(_Size) {}

    void Update(float _DeltaTime) override { ProcessLayout(); }

private:
    Vector2 m_Size;
    LayoutEngine m_LayoutEngine;

    void ProcessLayout() {
        auto roots = FindRoots();
        for (auto root : roots) {

            auto transform = m_Registry.GetComponent<TransformComponent>(root);
            transform->rect.width = GetScreenWidth() * GetWindowScaleDPI().x;   // m_Size.x;
            transform->rect.height = GetScreenHeight() * GetWindowScaleDPI().y; // m_Size.y

            MeasureEntityContent(root);
            ArrangeEntityContent(root);
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

    void MeasureEntityContent(EntityID _EntityID) {
        if (!m_LayoutEngine.TryMeasureEntityContent(_EntityID))
            return;

        for (const auto &child : m_Registry.GetComponent<ChildrenComponent>(_EntityID)->children) {
            MeasureEntityContent(child);
        }
    }

    void ArrangeEntityContent(EntityID _EntityID) {
        if (!m_LayoutEngine.TryArrangeEntityContent(_EntityID))
            return;

        for (const auto &child : m_Registry.GetComponent<ChildrenComponent>(_EntityID)->children) {
            ArrangeEntityContent(child);
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
};
} // namespace turnip::ecs
