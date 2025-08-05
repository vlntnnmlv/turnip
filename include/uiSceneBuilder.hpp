// Copyright 2025 Valentin Namleev

#pragma once

#include "./ecs/components/childrenComponent.hpp"
#include "./ecs/components/colorComponent.hpp"
#include "./ecs/components/layoutComponent.hpp"
#include "./ecs/components/parentComponent.hpp"
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
        m_Registry.AddComponent<ecs::LayoutComponent>(node, _Size);

        return node;
    }

    ecs::EntityID CreateScene(LRTB _Padding = {0, 0, 0, 0}) {
        return CreateNode(ecs::NullEntity, Size{SizeType::FILL, SizeType::FILL}, {0, 0, 0, 0},
                          _Padding);
    }

    ecs::EntityID CreateImage(ecs::EntityID _Parent, Texture2D _Texture, LRTB _Patch,
                              Color _Color = {255, 255, 255, 255},
                              Size _Size = Size{SizeType::FILL, SizeType::FILL},
                              LRTB _Margin = {0, 0, 0, 0}, LRTB _Padding = {0, 0, 0, 0}) {
        ecs::EntityID node = CreateNode(_Parent, _Size, _Margin, _Padding);

        m_Registry.AddComponent<ecs::SpriteComponent>(node, _Texture, _Patch);
        m_Registry.AddComponent<ecs::ColorComponent>(node, _Color);

        return node;
    }

private:
    ecs::Registry &m_Registry;

    void SetParent(ecs::EntityID _Child, ecs::EntityID _Parent) {
        std::cout << "C: " << _Child << ", P: " << _Parent << "\n";
        m_Registry.AddComponent<ecs::ParentComponent>(_Child);

        ecs::ChildrenComponent *childrenComponent =
            m_Registry.GetComponent<ecs::ChildrenComponent>(_Parent);

        std::cout << childrenComponent << "\n";

        if (!childrenComponent)
            m_Registry.AddComponent<ecs::ChildrenComponent>(_Parent);

        childrenComponent = m_Registry.GetComponent<ecs::ChildrenComponent>(_Parent);

        std::cout << childrenComponent << "\n";
        std::cout << childrenComponent << " " << &childrenComponent->children << "\n";

        childrenComponent->children.push_back(_Child);
    }
};
} // namespace turnip
