// Copyright 2025 Valentin Namleev

#pragma once

#include "../../axis.hpp"
#include "../../events/eventQueue.hpp"
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
    UISystem(Registry &_Registry, events::EventQueue &_EventQueue, Vector2 _Size)
        : ISystem(_Registry), m_LayoutEngine(m_Registry), m_EventQueue(_EventQueue), m_Size(_Size) {
    }

    void Update(float _DeltaTime) override {
        ProcessLayout();
        PollEvents();
    }

private:
    events::EventQueue &m_EventQueue;
    Vector2 m_Size;
    LayoutEngine m_LayoutEngine;
    bool m_WasResized;

    void PollEvents() {
        while (auto event = m_EventQueue.Pop()) {
        }
    }

    void ProcessLayout() {
        auto roots = FindRoots();
        m_WasResized |= IsWindowResized();

        for (auto root : roots) {
            auto transform = m_Registry.GetComponent<TransformComponent>(root);

            if (!m_WasResized) {
                transform->rect.width = m_Size.x;
                transform->rect.height = m_Size.y;
            } else {
                transform->rect.width = GetRenderWidth();
                transform->rect.height = GetRenderHeight();
            }

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
