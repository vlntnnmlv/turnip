// Copyright 2025 Valentin Namleev

#pragma once

// #include "feyerverx/axis.hpp"
// #include "feyerverx/ecs/components/buttonComponent.hpp"
// #include "feyerverx/ecs/components/hoverComponent.hpp"
// #include "feyerverx/ecs/components/layoutComponent.hpp"
// #include "feyerverx/ecs/components/parentComponent.hpp"
// #include "feyerverx/ecs/components/stackComponent.hpp"
// #include "feyerverx/ecs/components/transformComponent.hpp"
// #include "feyerverx/ecs/engines/layoutEngine.hpp"
// #include "feyerverx/ecs/registry.hpp"
#include "feyerverx/ecs/engines/layoutEngine.hpp"
#include "feyerverx/ecs/system.hpp"
// #include "feyerverx/events/eventQueue.hpp"
// // #include "feyerverx/rectangleUtils.hpp"

// #include <iostream>
// #include <optional>
// #include <type_traits>
// #include <unordered_map>

namespace feyerverx::ecs {
class UISystem final : public ISystem {
public:
    UISystem() = default;
    ~UISystem() override = default;

    void update([[maybe_unused]] float deltaTime) override;
    void enqueueScene(Scene &scene) override;

    Vector2f m_size{};

private:
    std::vector<Entity> m_roots;
    LayoutEngine m_layoutEngine{};

    void processLayout();

    void measureEntityContent(Entity entity);
    void arrangeEntityContent(Entity entity);
    void placeInWorld(Entity entity);
};
}
// public:
//     UISystem(Registry &_Registry, events::EventQueue &_EventQueue, Vector2f _Size);

//     void update(float _DeltaTime) override;

// private:
//     events::EventQueue &m_EventQueue;
//     LayoutEngine m_LayoutEngine;

//     Vector2f m_Size;
//     bool m_WasResized;

//     EntityID m_HoveredEntityID = ecs::NullEntityID;
//     EntityID m_PressedEntityID = ecs::NullEntityID;

//     void PollEvents();
//     void OnMouseEvent(events::InputEvent &_Event, EntityID _UIRootEntityID);

//     void SetHoveredEntity(EntityID _EntityID);
//     void HoverdEffect(bool _Enable);
//     void SetPressedEntity(EntityID _EntityID);
//     void PressedEffect(bool _Enable);

//     void TryUseButton(EntityID _EntityID);

//     template <typename TComponent = void>
//     EntityID FindEventHit(EntityID _EntityID, const events::InputEvent &_Event) {
//         auto *transform = m_registry.GetComponent<TransformComponent>(_EntityID);

//         // if (!transform || !transform->worldRect.CheckCollision(_Event.position))
//         //     return ecs::NullEntityID;

//         // if (auto *children = m_registry.GetComponent<ChildrenComponent>(_EntityID)) {
//         //     // Reverse order so the *last* child wins.
//         //     for (auto it = children->children.rbegin(); it != children->children.rend(); ++it)
//         {
//         //         if (EntityID hit = FindEventHit<TComponent>(*it, _Event); hit !=
//         //         ecs::NullEntityID)
//         //             return hit;
//         //     }
//         // }

//         // if constexpr (std::is_void_v<TComponent>) {
//         //     // No filter requested → any entity counts.
//         //     std::cout << "is void!\n";
//         //     return _EntityID;
//         // } else {
//         //     // Filtered mode → only entities that have TComponent.
//         //     return m_registry.GetComponent<TComponent>(_EntityID) ? _EntityID :
//         //     ecs::NullEntityID;
//         // }
//     }

//     void ProcessLayout();

//     std::vector<EntityID> FindRoots();

//     void MeasureEntityContent(EntityID _EntityID);
//     void ArrangeEntityContent(EntityID _EntityID);
//     void PlaceInWorld(EntityID _EntityID);
// };
// } // namespace feyerverx::ecs
