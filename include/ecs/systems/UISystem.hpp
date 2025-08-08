// Copyright 2025 Valentin Namleev

#pragma once

#include "../../axis.hpp"
#include "../../events/eventQueue.hpp"
#include "../../rectangleUtils.hpp"
#include "../components/buttonComponent.hpp"
#include "../components/hoverComponent.hpp"
#include "../components/layoutComponent.hpp"
#include "../components/parentComponent.hpp"
#include "../components/stackComponent.hpp"
#include "../components/transformComponent.hpp"
#include "../engines/layoutEngine.hpp"
#include "../registry.hpp"
#include "../system.hpp"

#include <optional>
#include <type_traits>
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
    LayoutEngine m_LayoutEngine;

    Vector2 m_Size;
    bool m_WasResized;

    EntityID m_HoveredEntity = ecs::NullEntity;
    EntityID m_PressedEntity = ecs::NullEntity;

    void PollEvents() {
        while (auto optionalEvent = m_EventQueue.Pop()) {

            if (!optionalEvent.has_value())
                continue;

            events::InputEvent event = optionalEvent.value();
            auto roots = FindRoots();

            for (auto root : roots) {
                OnMouseEvent(event, root);
            }
        }
    }

    void OnMouseEvent(events::InputEvent &_Event, EntityID _UIRootEntityID) {
        EntityID hit;

        switch (_Event.type) {
        case events::InputEventType::PRESSED:
            hit = FindEventHit<ButtonComponent>(_UIRootEntityID, _Event);
            SetPressedEntity(hit);
            break;
        case events::InputEventType::RELEASED:
            hit = FindEventHit<ButtonComponent>(_UIRootEntityID, _Event);
            TryUseButton(hit);
            break;
        case events::InputEventType::MOVED:
            hit = FindEventHit<HoverComponent>(_UIRootEntityID, _Event);
            SetHoveredEntity(hit);
            break;
        default:
            break;
        }
    }

    void SetHoveredEntity(EntityID _EntityID) {
        if (m_HoveredEntity == _EntityID)
            return;

        if (m_HoveredEntity != ecs::NullEntity)
            HoverdEffect(false);

        if (m_PressedEntity != _EntityID)
            SetPressedEntity(ecs::NullEntity);

        m_HoveredEntity = _EntityID;
        HoverdEffect(true);
    }

    void HoverdEffect(bool _Enable) {
        RenderTransformComponent *rtc =
            m_Registry.GetComponent<RenderTransformComponent>(m_HoveredEntity);

        if (rtc)
            rtc->rectOffset = _Enable ? LRTB{5, 5, 5, 5} : LRTB{0, 0, 0, 0};
    }

    void SetPressedEntity(EntityID _EntityID) {
        if (m_PressedEntity == _EntityID)
            return;

        m_PressedEntity = _EntityID;
    }

    void TryUseButton(EntityID _EntityID) {
        if (_EntityID != m_PressedEntity)
            return;

        ButtonComponent *buttonComponent = m_Registry.GetComponent<ButtonComponent>(_EntityID);

        if (buttonComponent) {
            buttonComponent->onClick();
        }

        SetPressedEntity(ecs::NullEntity);
    }

    template <typename TComponent = void>
    EntityID FindEventHit(EntityID _EntityID, const events::InputEvent &_Event) {
        auto *transform = m_Registry.GetComponent<TransformComponent>(_EntityID);

        if (!transform || !transform->worldRect.CheckCollision(_Event.position))
            return ecs::NullEntity;

        if (auto *children = m_Registry.GetComponent<ChildrenComponent>(_EntityID)) {
            // Reverse order so the *last* child wins.
            for (auto it = children->children.rbegin(); it != children->children.rend(); ++it) {
                if (EntityID hit = FindEventHit<TComponent>(*it, _Event); hit != ecs::NullEntity)
                    return hit;
            }
        }

        if constexpr (std::is_void_v<TComponent>) {
            // No filter requested → any entity counts.
            std::cout << "is void!\n";
            return _EntityID;
        } else {
            // Filtered mode → only entities that have TComponent.
            return m_Registry.GetComponent<TComponent>(_EntityID) ? _EntityID : ecs::NullEntity;
        }
    }

    // EntityID FindEventHit(EntityID _EntityID, const events::InputEvent &_Event) {
    //     TransformComponent *transformComponent =
    //         m_Registry.GetComponent<TransformComponent>(_EntityID);

    //     if (!transformComponent ||
    //     !transformComponent->worldRect.CheckCollision(_Event.position))
    //         return ecs::NullEntity;

    //     ChildrenComponent *childrenComponent =
    //         m_Registry.GetComponent<ChildrenComponent>(_EntityID);

    //     if (childrenComponent) {
    //         for (auto const &child : childrenComponent->children) {
    //             EntityID hit = FindEventHit(child, _Event);
    //             if (hit != ecs::NullEntity)
    //                 return hit;
    //         }
    //     }

    //     return _EntityID;
    // }

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
