// Copyright 2025 Valentin Namleev

#pragma once

#include <functional>

#include "turnip/ecs/components/childrenComponent.hpp"
#include "turnip/ecs/components/colorComponent.hpp"
#include "turnip/ecs/components/hoverComponent.hpp"
#include "turnip/ecs/components/layoutComponent.hpp"
#include "turnip/ecs/components/parentComponent.hpp"
#include "turnip/ecs/components/renderTransformComponent.hpp"
#include "turnip/ecs/components/spriteComponent.hpp"
#include "turnip/ecs/components/stackComponent.hpp"
#include "turnip/ecs/components/transformComponent.hpp"
#include "turnip/ecs/entity.hpp"
#include "turnip/ecs/registry.hpp"

namespace turnip {
class UISceneBuilder {
public:
    UISceneBuilder(ecs::Registry &_Registry);
    ~UISceneBuilder() = default;

    ecs::Entity CreateNode(ecs::Entity _Parent, Size _Size = Size{SizeType::FILL, SizeType::FILL},
                           LRTB _Margin = {0, 0, 0, 0}, LRTB _Padding = {0, 0, 0, 0});

    ecs::Entity CreateScene(LRTB _Padding = {0, 0, 0, 0});

    ecs::Entity CreateImage(ecs::Entity _Parent, raylib::Texture2D &_Texture,
                            LRTB _Patch = {0, 0, 0, 0}, raylib::Color _Color = {255, 255, 255, 255},
                            Size _Size = Size{SizeType::FILL, SizeType::FILL},
                            LRTB _Margin = {0, 0, 0, 0}, LRTB _Padding = {0, 0, 0, 0});

    ecs::Entity CreateStack(ecs::Entity _Parent, ecs::StackType _StackType,
                            ecs::StackContentType _StackContentType = ecs::StackContentType::CENTER,
                            float _Spacing = 0, Size _Size = Size{SizeType::FILL, SizeType::FILL},
                            LRTB _Margin = {0, 0, 0, 0}, LRTB _Padding = {0, 0, 0, 0});

    ecs::Entity CreateText(ecs::Entity _Parent, const std::string &_Text, raylib::Font &_Font,
                           float _FontSize = 24, float _Spacing = 5,
                           raylib::Color _Color = {255, 255, 255, 255},
                           Size _Size = Size{SizeType::FILL, SizeType::FILL},
                           LRTB _Margin = {0, 0, 0, 0}, LRTB _Padding = {0, 0, 0, 0});

    // TODO: Think how to make this better!
    ecs::Entity CreateButton(ecs::Entity _Parent, std::function<void()> _OnClick,
                             raylib::Texture2D &_Texture, LRTB _Patch = {0, 0, 0, 0},
                             raylib::Color _Color = {255, 255, 255, 255},
                             Size _Size = Size{SizeType::FILL, SizeType::FILL},
                             LRTB _Margin = {0, 0, 0, 0}, LRTB _Padding = {0, 0, 0, 0});

    ecs::Entity CreateLabeledButton(ecs::Entity _Parent, std::function<void()> _OnClick,
                                    raylib::Texture2D &_Texture, const std::string &_Text,
                                    raylib::Font &_Font, float _FontSize = 24, float _Spacing = 5,
                                    LRTB _Patch = {0, 0, 0, 0},
                                    raylib::Color _Color = {255, 255, 255, 255},
                                    Size _Size = Size{SizeType::FILL, SizeType::FILL},
                                    LRTB _Margin = {0, 0, 0, 0}, LRTB _Padding = {0, 0, 0, 0});

private:
    ecs::Registry &m_Registry;
    void SetParent(ecs::Entity _Child, ecs::Entity _Parent);
};
} // namespace turnip
