// Copyright 2025 Valentin Namleev

#include "feyerverx/ecs/systems/uiSystem.hpp"

#include "feyerverx/ecs/components/parentComponent.hpp"
// #include "feyerverx/ecs/components/renderTransformComponent.hpp"

// UISystem::UISystem(Registry &_Registry, events::EventQueue &_EventQueue, Vector2f _Size)
//     : ISystem(_Registry), m_EventQueue(_EventQueue), m_LayoutEngine(m_registry), m_Size(_Size) {}

namespace feyerverx::ecs {
void UISystem::update(float deltaTime) {
    processLayout();
    m_roots.clear();
}
void UISystem::enqueueScene(Scene &scene) {
    for (const auto &e : scene.registry().with<TransformComponent, LayoutComponent>()) {
        if (!scene.registry().getComponent<ParentComponent>(e)) {
            m_roots.emplace_back(e, &scene.registry());
        }
    }
}

void UISystem::processLayout() {
    // m_WasResized |= IsWindowResized();

    for (auto root : m_roots) {
        auto transformComponent = root.getComponent<TransformComponent>();

        // if (!m_wasResized) {
        transformComponent->rect.width = m_size.x;
        transformComponent->rect.height = m_size.y;
        // } else {
        //     transformComponent->rect.width = GetRenderWidth();
        //     transformComponent->rect.height = GetRenderHeight();
        // }

        measureEntityContent(root);
        arrangeEntityContent(root);
        placeInWorld(root);
    }
}

void UISystem::measureEntityContent(Entity entity) {
    if (!m_layoutEngine.tryMeasureEntityContent(entity))
        return;

    for (const auto &child : entity.getComponent<ChildrenComponent>()->children.value()) {
        measureEntityContent(child);
    }
}

void UISystem::arrangeEntityContent(Entity entity) {
    if (!m_layoutEngine.tryArrangeEntityContent(entity))
        return;

    for (const auto &child : entity.getComponent<ChildrenComponent>()->children.value()) {
        arrangeEntityContent(child);
    }
}

void UISystem::placeInWorld(Entity entity) {
    TransformComponent *transformComponent = entity.getComponent<TransformComponent>();
    ParentComponent *parentComponent = entity.getComponent<ParentComponent>();
    ChildrenComponent *childrenComponent = entity.getComponent<ChildrenComponent>();

    if (!parentComponent)
        transformComponent->worldRect = transformComponent->rect;
    else {
        TransformComponent *parentTransformComponent =
            parentComponent->parent.getComponent<TransformComponent>();
        transformComponent->worldRect = transformComponent->rect.move(
            Vector2f{parentTransformComponent->worldRect.x, parentTransformComponent->worldRect.y});
    }

    if (!childrenComponent)
        return;

    for (const auto &child : childrenComponent->children.value()) {
        placeInWorld(child);
    }
}
} // feyerverx::ecs
// void UISystem::PollEvents() {
//     while (auto optionalEvent = m_EventQueue.Pop()) {

//         if (!optionalEvent.has_value())
//             continue;

//         events::InputEvent event = optionalEvent.value();
//         auto roots = FindRoots();

//         for (auto root : roots) {
//             OnMouseEvent(event, root);
//         }
//     }
// }

// void UISystem::OnMouseEvent(events::InputEvent &_Event, EntityID _UIRootEntityID) {
//     EntityID hit;

//     switch (_Event.type) {
//     case events::InputEventType::PRESSED:
//         hit = FindEventHit<ButtonComponent>(_UIRootEntityID, _Event);
//         SetPressedEntity(hit);
//         break;
//     case events::InputEventType::RELEASED:
//         hit = FindEventHit<ButtonComponent>(_UIRootEntityID, _Event);
//         TryUseButton(hit);
//         break;
//     case events::InputEventType::MOVED:
//         hit = FindEventHit<HoverComponent>(_UIRootEntityID, _Event);
//         SetHoveredEntity(hit);
//         break;
//     default:
//         break;
//     }
// }

// void UISystem::SetHoveredEntity(EntityID _EntityID) {
//     if (m_HoveredEntityID == _EntityID)
//         return;

//     if (m_HoveredEntityID != ecs::NullEntityID)
//         HoverdEffect(false);

//     if (m_PressedEntityID != _EntityID)
//         SetPressedEntity(ecs::NullEntityID);

//     m_HoveredEntityID = _EntityID;
//     HoverdEffect(true);
// }

// void UISystem::HoverdEffect(bool _Enable) {
//     RenderTransformComponent *rtc =
//         m_registry.GetComponent<RenderTransformComponent>(m_HoveredEntityID);

//     if (rtc)
//         rtc->rectOffset = _Enable ? LRTB{5, 5, 5, 5} : LRTB{0, 0, 0, 0};
// }

// void UISystem::SetPressedEntity(EntityID _EntityID) {
//     if (m_PressedEntityID == _EntityID)
//         return;

//     if (m_PressedEntityID != ecs::NullEntityID)
//         PressedEffect(false);

//     m_PressedEntityID = _EntityID;
//     PressedEffect(true);
// }

// void UISystem::PressedEffect(bool _Enable) {
//     ButtonComponent *buttonComponent =
//     m_registry.GetComponent<ButtonComponent>(m_PressedEntityID);

//     if (!buttonComponent)
//         return;

//     RenderTransformComponent *rtc =
//         m_registry.GetComponent<RenderTransformComponent>(buttonComponent->image.ID());

//     if (rtc)
//         rtc->rectOffset = _Enable ? LRTB{-5, -5, -5, -5} : LRTB{0, 0, 0, 0};
// }

// void UISystem::TryUseButton(EntityID _EntityID) {
//     if (_EntityID != m_PressedEntityID)
//         return;

//     ButtonComponent *buttonComponent = m_registry.GetComponent<ButtonComponent>(_EntityID);

//     if (buttonComponent) {
//         buttonComponent->onClick();
//     }

//     SetPressedEntity(ecs::NullEntityID);
// }

// void UISystem::ProcessLayout() {
//     auto roots = FindRoots();
//     // m_WasResized |= IsWindowResized();

//     // for (auto root : roots) {
//     //     auto transformComponent = m_registry.GetComponent<TransformComponent>(root);

//     //     if (!m_WasResized) {
//     //         transformComponent->rect.width = m_Size.x;
//     //         transformComponent->rect.height = m_Size.y;
//     //     } else {
//     //         transformComponent->rect.width = GetRenderWidth();
//     //         transformComponent->rect.height = GetRenderHeight();
//     //     }

//     //     MeasureEntityContent(root);
//     //     ArrangeEntityContent(root);
//     //     PlaceInWorld(root);
//     // }
// }

// std::vector<EntityID> UISystem::FindRoots() {
//     std::vector<EntityID> roots;

//     for (const auto &e : m_registry.With<TransformComponent, LayoutComponent>()) {
//         if (!m_registry.GetComponent<ParentComponent>(e)) {
//             roots.push_back(e);
//         }
//     }

//     return roots;
// }

// } // namespace feyerverx::ecs
