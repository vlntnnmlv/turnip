// Copyright 2025 Valentin Namleev

#include "feyerverx/uiBuilder.hpp"
#include "feyerverx/ecs/components/buttonComponent.hpp"
#include "feyerverx/ecs/components/hoverComponent.hpp"
#include "feyerverx/ecs/components/parentComponent.hpp"
#include "feyerverx/ecs/components/textComponent.hpp"
#include "feyerverx/ecs/components/transformComponent.hpp"
#include "feyerverx/ecs/entity.hpp"

namespace feyerverx {
UIBuilder::UIBuilder(std::shared_ptr<ecs::Registry> registry) : m_registry{registry} {}

ecs::Entity UIBuilder::node(ecs::Entity parent, Size size, RectangleOffset margin, RectangleOffset padding) {
    ecs::Entity node = m_registry->createEntity();

    if (parent != ecs::NullEntityID) {
        setParent(node, parent);
    }

    node.addComponent<ecs::TransformComponent>();
    node.addComponent<ecs::LayoutComponent>(size, margin, padding);
    node.addComponent<ecs::RenderTransformComponent>();
    return node;
}

ecs::Entity UIBuilder::root(RectangleOffset padding) {
    return node(ecs::Entity(), Size{SizeType::FILL, SizeType::FILL}, {0, 0, 0, 0}, padding);
}

ecs::Entity UIBuilder::image(ecs::Entity parent, Texture &texture, RectangleOffset _Patch, Color color,
                             Size _Size, RectangleOffset _Margin, RectangleOffset _Padding) {
    ecs::Entity image = node(parent, _Size, _Margin, _Padding);

    image.addComponent<ecs::SpriteComponent>(texture, _Patch);
    image.addComponent<ecs::ColorComponent>(color);

    return image;
}

ecs::Entity UIBuilder::stack(ecs::Entity _Parent, ecs::StackType _StackType,
                             ecs::StackContentType _StackContentType, float _Spacing, Size _Size,
                             RectangleOffset _Margin, RectangleOffset _Padding) {
    ecs::Entity stack = node(_Parent, _Size, _Margin, _Padding);

    stack.addComponent<ecs::StackComponent>(_StackType, _StackContentType, _Spacing);

    return stack;
}

ecs::Entity UIBuilder::text(ecs::Entity _Parent, const std::string &_Text, Font &_Font,
                            float _FontSize, float _Spacing, Color _Color, Size _Size, RectangleOffset _Margin,
                            RectangleOffset _Padding) {
    ecs::Entity text = node(_Parent, _Size, _Margin, _Padding);

    text.addComponent<ecs::TextComponent>(_Text, _Font, _FontSize, _Spacing);
    text.addComponent<ecs::ColorComponent>(_Color);

    return text;
}

// TODO: Think how to make this better!
ecs::Entity UIBuilder::button(ecs::Entity _Parent, std::function<void()> _OnClick,
                              Texture &_Texture, RectangleOffset _Patch, Color _Color, Size _Size,
                              RectangleOffset _Margin, RectangleOffset _Padding) {
    ecs::Entity button = node(_Parent, _Size, _Margin, _Padding);
    ecs::Entity img = node(button);

    img.addComponent<ecs::SpriteComponent>(_Texture, _Patch);
    img.addComponent<ecs::ColorComponent>(_Color);
    img.addComponent<ecs::HoverComponent>();

    button.addComponent<ecs::ButtonComponent>(_OnClick, img);
    return button;
}

ecs::Entity UIBuilder::labeledButton(ecs::Entity _Parent, std::function<void()> _OnClick,
                                     Texture &_Texture, const std::string &_Text, Font &_Font,
                                     float _FontSize, float _Spacing, RectangleOffset _Patch, Color _Color,
                                     Size _Size, RectangleOffset _Margin, RectangleOffset _Padding) {
    ecs::Entity button = node(_Parent, _Size, _Margin, _Padding);
    ecs::Entity img = node(button);

    img.addComponent<ecs::SpriteComponent>(_Texture, _Patch);
    img.addComponent<ecs::ColorComponent>(_Color);
    img.addComponent<ecs::HoverComponent>();

    button.addComponent<ecs::TextComponent>(_Text, _Font, _FontSize, _Spacing);
    button.addComponent<ecs::ButtonComponent>(_OnClick, img);
    return button;
}

void UIBuilder::setParent(ecs::Entity child, ecs::Entity parent) {
    child.addComponent<ecs::ParentComponent>(parent);

    ecs::ChildrenComponent *childrenComponent = parent.getComponent<ecs::ChildrenComponent>();

    if (!childrenComponent)
        parent.addComponent<ecs::ChildrenComponent>();

    childrenComponent = parent.getComponent<ecs::ChildrenComponent>();
    childrenComponent->children->push_back(child);
}
} // namespace feyerverx
