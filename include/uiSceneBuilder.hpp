// Copyright 2025 Valentin Namleev

#pragma once

#include "./ecs/components/childrenComponent.hpp"
#include "./ecs/components/colorComponent.hpp"
#include "./ecs/components/hoverComponent.hpp"
#include "./ecs/components/layoutComponent.hpp"
#include "./ecs/components/parentComponent.hpp"
#include "./ecs/components/renderTransformComponent.hpp"
#include "./ecs/components/spriteComponent.hpp"
#include "./ecs/components/stackComponent.hpp"
#include "./ecs/components/transformComponent.hpp"

#include "./ecs/registry.hpp"

namespace turnip {
class UISceneBuilder {
public:
    UISceneBuilder(ecs::Registry &_Registry) : m_Registry(_Registry) {}
    ~UISceneBuilder() = default;

    ecs::EntityID CreateNode(ecs::EntityID _Parent,
                             Size _Size = Size{SizeType::FILL, SizeType::FILL},
                             LRTB _Margin = {0, 0, 0, 0}, LRTB _Padding = {0, 0, 0, 0}) {
        ecs::EntityID node = m_Registry.CreateEntity();

        if (_Parent != ecs::NullEntity) {
            SetParent(node, _Parent);
        }

        m_Registry.AddComponent<ecs::TransformComponent>(node);
        m_Registry.AddComponent<ecs::LayoutComponent>(node, _Size, _Margin, _Padding);
        m_Registry.AddComponent<ecs::RenderTransformComponent>(node);
        return node;
    }

    ecs::EntityID CreateScene(LRTB _Padding = {0, 0, 0, 0}) {
        return CreateNode(ecs::NullEntity, Size{SizeType::FILL, SizeType::FILL}, {0, 0, 0, 0},
                          _Padding);
    }

    ecs::EntityID CreateImage(ecs::EntityID _Parent, raylib::Texture2D &_Texture,
                              LRTB _Patch = {0, 0, 0, 0},
                              raylib::Color _Color = {255, 255, 255, 255},
                              Size _Size = Size{SizeType::FILL, SizeType::FILL},
                              LRTB _Margin = {0, 0, 0, 0}, LRTB _Padding = {0, 0, 0, 0}) {
        ecs::EntityID node = CreateNode(_Parent, _Size, _Margin, _Padding);

        m_Registry.AddComponent<ecs::SpriteComponent>(node, _Texture, _Patch);
        m_Registry.AddComponent<ecs::ColorComponent>(node, _Color);

        return node;
    }

    ecs::EntityID
    CreateStack(ecs::EntityID _Parent, ecs::StackType _StackType,
                ecs::StackContentType _StackContentType = ecs::StackContentType::CENTER,
                float _Spacing = 0, Size _Size = Size{SizeType::FILL, SizeType::FILL},
                LRTB _Margin = {0, 0, 0, 0}, LRTB _Padding = {0, 0, 0, 0}) {
        ecs::EntityID node = CreateNode(_Parent, _Size, _Margin, _Padding);

        m_Registry.AddComponent<ecs::StackComponent>(node, _StackType, _StackContentType, _Spacing);

        return node;
    }

    ecs::EntityID CreateText(ecs::EntityID _Parent, const std::string &_Text, raylib::Font &_Font,
                             float _FontSize = 24, float _Spacing = 5,
                             raylib::Color _Color = {255, 255, 255, 255},
                             Size _Size = Size{SizeType::FILL, SizeType::FILL},
                             LRTB _Margin = {0, 0, 0, 0}, LRTB _Padding = {0, 0, 0, 0}) {
        ecs::EntityID node = CreateNode(_Parent, _Size, _Margin, _Padding);

        m_Registry.AddComponent<ecs::TextComponent>(node, _Text, _Font, _FontSize, _Spacing);
        m_Registry.AddComponent<ecs::ColorComponent>(node, _Color);

        return node;
    }

    // TODO: Think how to make this better!
    ecs::EntityID CreateButton(ecs::EntityID _Parent, std::function<void()> _OnClick,
                               raylib::Texture2D &_Texture, LRTB _Patch = {0, 0, 0, 0},
                               raylib::Color _Color = {255, 255, 255, 255},
                               Size _Size = Size{SizeType::FILL, SizeType::FILL},
                               LRTB _Margin = {0, 0, 0, 0}, LRTB _Padding = {0, 0, 0, 0}) {
        ecs::EntityID node = CreateNode(_Parent, _Size, _Margin, _Padding);

        ecs::EntityID img = CreateNode(node);
        m_Registry.AddComponent<ecs::SpriteComponent>(img, _Texture, _Patch);
        m_Registry.AddComponent<ecs::ColorComponent>(img, _Color);
        m_Registry.AddComponent<ecs::HoverComponent>(img);

        m_Registry.AddComponent<ecs::ButtonComponent>(node, _OnClick, img);
        return node;
    }

private:
    ecs::Registry &m_Registry;

    void SetParent(ecs::EntityID _Child, ecs::EntityID _Parent) {
        m_Registry.AddComponent<ecs::ParentComponent>(_Child, _Parent);

        ecs::ChildrenComponent *childrenComponent =
            m_Registry.GetComponent<ecs::ChildrenComponent>(_Parent);

        if (!childrenComponent)
            m_Registry.AddComponent<ecs::ChildrenComponent>(_Parent);

        childrenComponent = m_Registry.GetComponent<ecs::ChildrenComponent>(_Parent);

        childrenComponent->children.push_back(_Child);
    }
};
} // namespace turnip
