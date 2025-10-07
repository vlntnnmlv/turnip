// Copyright 2025 Valentin Namleev

#include "feyerverx/uiBuilder.hpp"
#include "feyerverx/ecs/components/buttonComponent.hpp"
#include "feyerverx/ecs/components/hoverComponent.hpp"
#include "feyerverx/ecs/components/parentComponent.hpp"
#include "feyerverx/ecs/components/textComponent.hpp"
#include "feyerverx/ecs/components/transformComponent.hpp"
#include "feyerverx/ecs/entity.hpp"

namespace feyerverx {
uiBuilder::uiBuilder(ecs::Registry &_Registry) : m_registry(_Registry) {}

ecs::Entity uiBuilder::node(ecs::Entity _Parent, Size _Size, LRTB _Margin, LRTB _Padding) {
    ecs::Entity node = m_registry.get().createEntity();

    if (_Parent != ecs::NullEntityID) {
        setParent(node, _Parent);
    }

    node.addComponent<ecs::TransformComponent>();
    node.addComponent<ecs::LayoutComponent>(_Size, _Margin, _Padding);
    node.addComponent<ecs::RenderTransformComponent>();
    return node;
}

ecs::Entity uiBuilder::root(LRTB padding) {
    return node(ecs::Entity(), Size{SizeType::FILL, SizeType::FILL}, {0, 0, 0, 0}, padding);
}

ecs::Entity uiBuilder::image(ecs::Entity _Parent, Texture &_Texture, LRTB _Patch, Color _Color,
                             Size _Size, LRTB _Margin, LRTB _Padding) {
    ecs::Entity image = node(_Parent, _Size, _Margin, _Padding);

    image.addComponent<ecs::SpriteComponent>(_Texture, _Patch);
    image.addComponent<ecs::ColorComponent>(_Color);

    return image;
}

ecs::Entity uiBuilder::stack(ecs::Entity _Parent, ecs::StackType _StackType,
                             ecs::StackContentType _StackContentType, float _Spacing, Size _Size,
                             LRTB _Margin, LRTB _Padding) {
    ecs::Entity stack = node(_Parent, _Size, _Margin, _Padding);

    stack.addComponent<ecs::StackComponent>(_StackType, _StackContentType, _Spacing);

    return stack;
}

ecs::Entity uiBuilder::text(ecs::Entity _Parent, const std::string &_Text, Font &_Font,
                            float _FontSize, float _Spacing, Color _Color, Size _Size, LRTB _Margin,
                            LRTB _Padding) {
    ecs::Entity text = node(_Parent, _Size, _Margin, _Padding);

    text.addComponent<ecs::TextComponent>(_Text, _Font, _FontSize, _Spacing);
    text.addComponent<ecs::ColorComponent>(_Color);

    return text;
}

// TODO: Think how to make this better!
ecs::Entity uiBuilder::button(ecs::Entity _Parent, std::function<void()> _OnClick,
                              Texture &_Texture, LRTB _Patch, Color _Color, Size _Size,
                              LRTB _Margin, LRTB _Padding) {
    ecs::Entity button = node(_Parent, _Size, _Margin, _Padding);
    ecs::Entity img = node(button);

    img.addComponent<ecs::SpriteComponent>(_Texture, _Patch);
    img.addComponent<ecs::ColorComponent>(_Color);
    img.addComponent<ecs::HoverComponent>();

    button.addComponent<ecs::ButtonComponent>(_OnClick, img);
    return button;
}

ecs::Entity uiBuilder::labeledButton(ecs::Entity _Parent, std::function<void()> _OnClick,
                                     Texture &_Texture, const std::string &_Text, Font &_Font,
                                     float _FontSize, float _Spacing, LRTB _Patch, Color _Color,
                                     Size _Size, LRTB _Margin, LRTB _Padding) {
    ecs::Entity button = node(_Parent, _Size, _Margin, _Padding);
    ecs::Entity img = node(button);

    img.addComponent<ecs::SpriteComponent>(_Texture, _Patch);
    img.addComponent<ecs::ColorComponent>(_Color);
    img.addComponent<ecs::HoverComponent>();

    button.addComponent<ecs::TextComponent>(_Text, _Font, _FontSize, _Spacing);
    button.addComponent<ecs::ButtonComponent>(_OnClick, img);
    return button;
}

void uiBuilder::setParent(ecs::Entity _Child, ecs::Entity _Parent) {
    _Child.addComponent<ecs::ParentComponent>(_Parent);

    ecs::ChildrenComponent *childrenComponent = _Parent.getComponent<ecs::ChildrenComponent>();

    if (!childrenComponent)
        _Parent.addComponent<ecs::ChildrenComponent>();

    childrenComponent = _Parent.getComponent<ecs::ChildrenComponent>();
    childrenComponent->children->push_back(_Child);
}
} // namespace feyerverx
