// Copyright 2025 Valentin Namleev

#include "feyerverx/ecs/systems/uiSystem.hpp"
#include "feyerverx/ecs/components/parentComponent.hpp"

#include "feyerverx/events/eventManager.hpp"

namespace feyerverx::ecs {

UISystem::UISystem(const Vector2f size, EventManager &eventManager)
    : ISystem("ui_system", eventManager), m_size(size) {
    m_eventManager.subscribe<WindowResizedEvent>(
        [this](const WindowResizedEvent &event) { m_size = event.windowSize; });
}

void UISystem::update(float deltaTime, const std::shared_ptr<Registry> &registry) {
    findRoots(registry);
    processLayout();
    m_roots.clear();
}

void UISystem::findRoots(const std::shared_ptr<Registry> &registry) {
    for (const auto &e : registry->with<TransformComponent, LayoutComponent>()) {
        if (!registry->getComponent<ParentComponent>(e)) {
            m_roots.emplace_back(e, registry.get());
        }
    }
}

void UISystem::processLayout() {
    for (auto root : m_roots) {
        const auto transformComponent = root.getComponent<TransformComponent>();

        transformComponent->rect.width = m_size.x;
        transformComponent->rect.height = m_size.y;

        measureEntityContent(root);
        arrangeEntityContent(root);
        placeInWorld(root);
    }
}

void UISystem::measureEntityContent(const Entity entity) {
    if (!m_layoutEngine.tryMeasureEntityContent(entity))
        return;

    for (const auto &child : entity.getComponent<ChildrenComponent>()->children.value()) {
        measureEntityContent(child);
    }
}

void UISystem::arrangeEntityContent(const Entity entity) {
    if (!m_layoutEngine.tryArrangeEntityContent(entity))
        return;

    for (const auto &child : entity.getComponent<ChildrenComponent>()->children.value()) {
        arrangeEntityContent(child);
    }
}

void UISystem::placeInWorld(const Entity entity) {
    auto *transformComponent = entity.getComponent<TransformComponent>();
    const auto *parentComponent = entity.getComponent<ParentComponent>();
    const auto *childrenComponent = entity.getComponent<ChildrenComponent>();

    if (!parentComponent)
        transformComponent->worldRect = transformComponent->rect;
    else {
        const auto *parentTransformComponent =
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
} // namespace feyerverx