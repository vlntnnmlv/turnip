// Copyright 2025 Valentin Namleev

#pragma once

#include "./turnip/axis.hpp"
#include "./turnip/ecs/components/buttonComponent.hpp"
#include "./turnip/ecs/components/hoverComponent.hpp"
#include "./turnip/ecs/components/layoutComponent.hpp"
#include "./turnip/ecs/components/parentComponent.hpp"
#include "./turnip/ecs/components/stackComponent.hpp"
#include "./turnip/ecs/components/transformComponent.hpp"
#include "./turnip/ecs/engines/layoutEngine.hpp"
#include "./turnip/ecs/registry.hpp"
#include "./turnip/ecs/system.hpp"
#include "./turnip/events/eventQueue.hpp"
#include "./turnip/rectangleUtils.hpp"

#include <iostream>
#include <optional>
#include <type_traits>
#include <unordered_map>

namespace turnip::ecs {
class UISystem : protected ISystem {
public:
    UISystem(Registry &_Registry, events::EventQueue &_EventQueue, Vector2 _Size);

    void Update(float _DeltaTime) override;

private:
    events::EventQueue &m_EventQueue;
    LayoutEngine m_LayoutEngine;

    Vector2 m_Size;
    bool m_WasResized;

    EntityID m_HoveredEntity = ecs::NullEntity;
    EntityID m_PressedEntity = ecs::NullEntity;

    void PollEvents();
    void OnMouseEvent(events::InputEvent &_Event, EntityID _UIRootEntityID);

    void SetHoveredEntity(EntityID _EntityID);
    void HoverdEffect(bool _Enable);
    void SetPressedEntity(EntityID _EntityID);
    void PressedEffect(bool _Enable);

    void TryUseButton(EntityID _EntityID);

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

    void ProcessLayout();

    std::vector<EntityID> FindRoots();

    void MeasureEntityContent(EntityID _EntityID);
    void ArrangeEntityContent(EntityID _EntityID);
    void PlaceInWorld(EntityID _EntityID);
};
} // namespace turnip::ecs
