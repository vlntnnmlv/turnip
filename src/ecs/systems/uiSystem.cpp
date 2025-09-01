// Copyright 2025 Valentin Namleev

#include "./turnip/ecs/systems/uiSystem.hpp"
#include "./turnip/ecs/components/renderTransformComponent.hpp"

namespace turnip::ecs {
UISystem::UISystem(Registry &_Registry, events::EventQueue &_EventQueue, Vector2 _Size)
    : ISystem(_Registry), m_EventQueue(_EventQueue), m_LayoutEngine(m_Registry), m_Size(_Size) {}

void UISystem::Update([[maybe_unused]] float _DeltaTime) {
    ProcessLayout();
    PollEvents();
}

void UISystem::PollEvents() {
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

void UISystem::OnMouseEvent(events::InputEvent &_Event, EntityID _UIRootEntityID) {
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

void UISystem::SetHoveredEntity(EntityID _EntityID) {
    if (m_HoveredEntity == _EntityID)
        return;

    if (m_HoveredEntity != ecs::NullEntity)
        HoverdEffect(false);

    if (m_PressedEntity != _EntityID)
        SetPressedEntity(ecs::NullEntity);

    m_HoveredEntity = _EntityID;
    HoverdEffect(true);
}

void UISystem::HoverdEffect(bool _Enable) {
    RenderTransformComponent *rtc =
        m_Registry.GetComponent<RenderTransformComponent>(m_HoveredEntity);

    if (rtc)
        rtc->rectOffset = _Enable ? LRTB{5, 5, 5, 5} : LRTB{0, 0, 0, 0};
}

void UISystem::SetPressedEntity(EntityID _EntityID) {
    if (m_PressedEntity == _EntityID)
        return;

    if (m_PressedEntity != ecs::NullEntity)
        PressedEffect(false);

    m_PressedEntity = _EntityID;
    PressedEffect(true);
}

void UISystem::PressedEffect(bool _Enable) {
    ButtonComponent *buttonComponent = m_Registry.GetComponent<ButtonComponent>(m_PressedEntity);

    if (!buttonComponent)
        return;

    RenderTransformComponent *rtc =
        m_Registry.GetComponent<RenderTransformComponent>(buttonComponent->image);

    if (rtc)
        rtc->rectOffset = _Enable ? LRTB{-5, -5, -5, -5} : LRTB{0, 0, 0, 0};
}

void UISystem::TryUseButton(EntityID _EntityID) {
    if (_EntityID != m_PressedEntity)
        return;

    ButtonComponent *buttonComponent = m_Registry.GetComponent<ButtonComponent>(_EntityID);

    if (buttonComponent) {
        buttonComponent->onClick();
    }

    SetPressedEntity(ecs::NullEntity);
}

void UISystem::ProcessLayout() {
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

std::vector<EntityID> UISystem::FindRoots() {
    std::vector<EntityID> roots;

    for (const auto &e : m_Registry.With<TransformComponent>()) {
        if (!m_Registry.GetComponent<ParentComponent>(e)) {
            roots.push_back(e);
        }
    }

    return roots;
}

void UISystem::MeasureEntityContent(EntityID _EntityID) {
    if (!m_LayoutEngine.TryMeasureEntityContent(_EntityID))
        return;

    for (const auto &child : m_Registry.GetComponent<ChildrenComponent>(_EntityID)->children) {
        MeasureEntityContent(child);
    }
}

void UISystem::ArrangeEntityContent(EntityID _EntityID) {
    if (!m_LayoutEngine.TryArrangeEntityContent(_EntityID))
        return;

    for (const auto &child : m_Registry.GetComponent<ChildrenComponent>(_EntityID)->children) {
        ArrangeEntityContent(child);
    }
}

void UISystem::PlaceInWorld(EntityID _EntityID) {
    TransformComponent *transformComponent = m_Registry.GetComponent<TransformComponent>(_EntityID);

    ParentComponent *parentComponent = m_Registry.GetComponent<ParentComponent>(_EntityID);

    ChildrenComponent *childrenComponent = m_Registry.GetComponent<ChildrenComponent>(_EntityID);

    if (!parentComponent)
        transformComponent->worldRect = transformComponent->rect;
    else {
        TransformComponent *parentTransformComponent =
            m_Registry.GetComponent<TransformComponent>(parentComponent->parent);
        transformComponent->worldRect = RectangleUtils::Move(
            transformComponent->rect,
            Vector2{parentTransformComponent->worldRect.x, parentTransformComponent->worldRect.y});
    }

    if (!childrenComponent)
        return;

    for (const auto &child : childrenComponent->children) {
        PlaceInWorld(child);
    }
}
} // namespace turnip::ecs
