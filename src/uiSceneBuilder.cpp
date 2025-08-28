// Copyright 2025 Valentin Namleev

#include "./turnip/uiSceneBuilder.hpp"
#include "./turnip/ecs/components/buttonComponent.hpp"
#include "./turnip/ecs/components/textComponent.hpp"

namespace turnip {
UISceneBuilder::UISceneBuilder(ecs::Registry &_Registry) : m_Registry(_Registry) {}

ecs::EntityID UISceneBuilder::CreateNode(ecs::EntityID _Parent, Size _Size, LRTB _Margin,
                                         LRTB _Padding) {
    ecs::EntityID node = m_Registry.CreateEntity();

    if (_Parent != ecs::NullEntity) {
        SetParent(node, _Parent);
    }

    m_Registry.AddComponent<ecs::TransformComponent>(node);
    m_Registry.AddComponent<ecs::LayoutComponent>(node, _Size, _Margin, _Padding);
    m_Registry.AddComponent<ecs::RenderTransformComponent>(node);
    return node;
}

ecs::EntityID UISceneBuilder::CreateScene(LRTB _Padding) {
    return CreateNode(ecs::NullEntity, Size{SizeType::FILL, SizeType::FILL}, {0, 0, 0, 0},
                      _Padding);
}

ecs::EntityID UISceneBuilder::CreateImage(ecs::EntityID _Parent, raylib::Texture2D &_Texture,
                                          LRTB _Patch, raylib::Color _Color, Size _Size,
                                          LRTB _Margin, LRTB _Padding) {
    ecs::EntityID node = CreateNode(_Parent, _Size, _Margin, _Padding);

    m_Registry.AddComponent<ecs::SpriteComponent>(node, _Texture, _Patch);
    m_Registry.AddComponent<ecs::ColorComponent>(node, _Color);

    return node;
}

ecs::EntityID UISceneBuilder::CreateStack(ecs::EntityID _Parent, ecs::StackType _StackType,
                                          ecs::StackContentType _StackContentType, float _Spacing,
                                          Size _Size, LRTB _Margin, LRTB _Padding) {
    ecs::EntityID node = CreateNode(_Parent, _Size, _Margin, _Padding);

    m_Registry.AddComponent<ecs::StackComponent>(node, _StackType, _StackContentType, _Spacing);

    return node;
}

ecs::EntityID UISceneBuilder::CreateText(ecs::EntityID _Parent, const std::string &_Text,
                                         raylib::Font &_Font, float _FontSize, float _Spacing,
                                         raylib::Color _Color, Size _Size, LRTB _Margin,
                                         LRTB _Padding) {
    ecs::EntityID node = CreateNode(_Parent, _Size, _Margin, _Padding);

    m_Registry.AddComponent<ecs::TextComponent>(node, _Text, _Font, _FontSize, _Spacing);
    m_Registry.AddComponent<ecs::ColorComponent>(node, _Color);

    return node;
}

// TODO: Think how to make this better!
ecs::EntityID UISceneBuilder::CreateButton(ecs::EntityID _Parent, std::function<void()> _OnClick,
                                           raylib::Texture2D &_Texture, LRTB _Patch,
                                           raylib::Color _Color, Size _Size, LRTB _Margin,
                                           LRTB _Padding) {
    ecs::EntityID node = CreateNode(_Parent, _Size, _Margin, _Padding);

    ecs::EntityID img = CreateNode(node);
    m_Registry.AddComponent<ecs::SpriteComponent>(img, _Texture, _Patch);
    m_Registry.AddComponent<ecs::ColorComponent>(img, _Color);
    m_Registry.AddComponent<ecs::HoverComponent>(img);

    m_Registry.AddComponent<ecs::ButtonComponent>(node, _OnClick, img);
    return node;
}

void UISceneBuilder::SetParent(ecs::EntityID _Child, ecs::EntityID _Parent) {
    m_Registry.AddComponent<ecs::ParentComponent>(_Child, _Parent);

    ecs::ChildrenComponent *childrenComponent =
        m_Registry.GetComponent<ecs::ChildrenComponent>(_Parent);

    if (!childrenComponent)
        m_Registry.AddComponent<ecs::ChildrenComponent>(_Parent);

    childrenComponent = m_Registry.GetComponent<ecs::ChildrenComponent>(_Parent);

    childrenComponent->children.push_back(_Child);
}
} // namespace turnip
