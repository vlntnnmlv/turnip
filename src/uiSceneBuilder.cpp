// Copyright 2025 Valentin Namleev

#include "turnip/uiSceneBuilder.hpp"
#include "turnip/ecs/components/buttonComponent.hpp"
#include "turnip/ecs/components/textComponent.hpp"

namespace turnip {
UISceneBuilder::UISceneBuilder(ecs::Registry &_Registry) : m_Registry(_Registry) {}

ecs::Entity UISceneBuilder::CreateNode(ecs::Entity _Parent, Size _Size, LRTB _Margin,
                                       LRTB _Padding) {
    ecs::Entity node = m_Registry.CreateEntity();

    if (_Parent != ecs::NullEntityID) {
        SetParent(node, _Parent);
    }

    node.AddComponent<ecs::TransformComponent>();
    node.AddComponent<ecs::LayoutComponent>(_Size, _Margin, _Padding);
    node.AddComponent<ecs::RenderTransformComponent>();
    return node;
}

ecs::Entity UISceneBuilder::CreateScene(LRTB _Padding) {
    return CreateNode(ecs::Entity(), Size{SizeType::FILL, SizeType::FILL}, {0, 0, 0, 0}, _Padding);
}

ecs::Entity UISceneBuilder::CreateImage(ecs::Entity _Parent, raylib::Texture2D &_Texture,
                                        LRTB _Patch, raylib::Color _Color, Size _Size, LRTB _Margin,
                                        LRTB _Padding) {
    ecs::Entity image = CreateNode(_Parent, _Size, _Margin, _Padding);

    image.AddComponent<ecs::SpriteComponent>(_Texture, _Patch);
    image.AddComponent<ecs::ColorComponent>(_Color);

    return image;
}

ecs::Entity UISceneBuilder::CreateStack(ecs::Entity _Parent, ecs::StackType _StackType,
                                        ecs::StackContentType _StackContentType, float _Spacing,
                                        Size _Size, LRTB _Margin, LRTB _Padding) {
    ecs::Entity stack = CreateNode(_Parent, _Size, _Margin, _Padding);

    stack.AddComponent<ecs::StackComponent>(_StackType, _StackContentType, _Spacing);

    return stack;
}

ecs::Entity UISceneBuilder::CreateText(ecs::Entity _Parent, const std::string &_Text,
                                       raylib::Font &_Font, float _FontSize, float _Spacing,
                                       raylib::Color _Color, Size _Size, LRTB _Margin,
                                       LRTB _Padding) {
    ecs::Entity text = CreateNode(_Parent, _Size, _Margin, _Padding);

    text.AddComponent<ecs::TextComponent>(_Text, _Font, _FontSize, _Spacing);
    text.AddComponent<ecs::ColorComponent>(_Color);

    return text;
}

// TODO: Think how to make this better!
ecs::Entity UISceneBuilder::CreateButton(ecs::Entity _Parent, std::function<void()> _OnClick,
                                         raylib::Texture2D &_Texture, LRTB _Patch,
                                         raylib::Color _Color, Size _Size, LRTB _Margin,
                                         LRTB _Padding) {
    ecs::Entity node = CreateNode(_Parent, _Size, _Margin, _Padding);
    ecs::Entity img = CreateNode(node);

    img.AddComponent<ecs::SpriteComponent>(_Texture, _Patch);
    img.AddComponent<ecs::ColorComponent>(_Color);
    img.AddComponent<ecs::HoverComponent>();

    node.AddComponent<ecs::ButtonComponent>(_OnClick, img);
    return node;
}

void UISceneBuilder::SetParent(ecs::Entity _Child, ecs::Entity _Parent) {
    _Child.AddComponent<ecs::ParentComponent>(_Parent);

    ecs::ChildrenComponent *childrenComponent = _Parent.GetComponent<ecs::ChildrenComponent>();

    if (!childrenComponent)
        _Parent.AddComponent<ecs::ChildrenComponent>();

    childrenComponent = _Parent.GetComponent<ecs::ChildrenComponent>();
    childrenComponent->children.push_back(_Child);
}
} // namespace turnip
