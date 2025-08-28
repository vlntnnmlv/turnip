// Copyright 2025 Valentin Namleev

#pragma once

#include <functional>

#include "./turnip/ecs/components/childrenComponent.hpp"
#include "./turnip/ecs/components/colorComponent.hpp"
#include "./turnip/ecs/components/hoverComponent.hpp"
#include "./turnip/ecs/components/layoutComponent.hpp"
#include "./turnip/ecs/components/parentComponent.hpp"
#include "./turnip/ecs/components/renderTransformComponent.hpp"
#include "./turnip/ecs/components/spriteComponent.hpp"
#include "./turnip/ecs/components/stackComponent.hpp"
#include "./turnip/ecs/components/transformComponent.hpp"

#include "./turnip/ecs/registry.hpp"

namespace turnip {
class UISceneBuilder {
public:
    UISceneBuilder(ecs::Registry &_Registry);
    ~UISceneBuilder() = default;

    ecs::EntityID CreateNode(ecs::EntityID _Parent,
                             Size _Size = Size{SizeType::FILL, SizeType::FILL},
                             LRTB _Margin = {0, 0, 0, 0}, LRTB _Padding = {0, 0, 0, 0});

    ecs::EntityID CreateScene(LRTB _Padding = {0, 0, 0, 0});

    ecs::EntityID CreateImage(ecs::EntityID _Parent, raylib::Texture2D &_Texture,
                              LRTB _Patch = {0, 0, 0, 0},
                              raylib::Color _Color = {255, 255, 255, 255},
                              Size _Size = Size{SizeType::FILL, SizeType::FILL},
                              LRTB _Margin = {0, 0, 0, 0}, LRTB _Padding = {0, 0, 0, 0});

    ecs::EntityID
    CreateStack(ecs::EntityID _Parent, ecs::StackType _StackType,
                ecs::StackContentType _StackContentType = ecs::StackContentType::CENTER,
                float _Spacing = 0, Size _Size = Size{SizeType::FILL, SizeType::FILL},
                LRTB _Margin = {0, 0, 0, 0}, LRTB _Padding = {0, 0, 0, 0});

    ecs::EntityID CreateText(ecs::EntityID _Parent, const std::string &_Text, raylib::Font &_Font,
                             float _FontSize = 24, float _Spacing = 5,
                             raylib::Color _Color = {255, 255, 255, 255},
                             Size _Size = Size{SizeType::FILL, SizeType::FILL},
                             LRTB _Margin = {0, 0, 0, 0}, LRTB _Padding = {0, 0, 0, 0});

    // TODO: Think how to make this better!
    ecs::EntityID CreateButton(ecs::EntityID _Parent, std::function<void()> _OnClick,
                               raylib::Texture2D &_Texture, LRTB _Patch = {0, 0, 0, 0},
                               raylib::Color _Color = {255, 255, 255, 255},
                               Size _Size = Size{SizeType::FILL, SizeType::FILL},
                               LRTB _Margin = {0, 0, 0, 0}, LRTB _Padding = {0, 0, 0, 0});

private:
    ecs::Registry &m_Registry;

    void SetParent(ecs::EntityID _Child, ecs::EntityID _Parent);
};
} // namespace turnip
