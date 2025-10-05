// Copyright 2025 Valentin Namleev

#pragma once

#include <functional>

#include "feyerverx/ecs/components/childrenComponent.hpp"
#include "feyerverx/ecs/components/colorComponent.hpp"
#include "feyerverx/ecs/components/hoverComponent.hpp"
#include "feyerverx/ecs/components/layoutComponent.hpp"
#include "feyerverx/ecs/components/parentComponent.hpp"
#include "feyerverx/ecs/components/renderTransformComponent.hpp"
#include "feyerverx/ecs/components/spriteComponent.hpp"
#include "feyerverx/ecs/components/stackComponent.hpp"
#include "feyerverx/ecs/components/transformComponent.hpp"

#include "feyerverx/ecs/entity.hpp"
#include "feyerverx/ecs/registry.hpp"
#include "feyerverx/font.hpp"

namespace feyerverx {
class UISceneBuilder {
public:
    UISceneBuilder(ecs::Registry &_Registry);
    ~UISceneBuilder() = default;

    ecs::Entity CreateNode(ecs::Entity _Parent, Size _Size = Size{SizeType::FILL, SizeType::FILL},
                           LRTB _Margin = {0, 0, 0, 0}, LRTB _Padding = {0, 0, 0, 0});

    ecs::Entity CreateScene(LRTB _Padding = {0, 0, 0, 0});

    ecs::Entity CreateImage(ecs::Entity _Parent, Texture &_Texture, LRTB _Patch = {0, 0, 0, 0},
                            Color _Color = {255, 255, 255, 255},
                            Size _Size = Size{SizeType::FILL, SizeType::FILL},
                            LRTB _Margin = {0, 0, 0, 0}, LRTB _Padding = {0, 0, 0, 0});

    ecs::Entity CreateStack(ecs::Entity _Parent, ecs::StackType _StackType,
                            ecs::StackContentType _StackContentType = ecs::StackContentType::CENTER,
                            float _Spacing = 0, Size _Size = Size{SizeType::FILL, SizeType::FILL},
                            LRTB _Margin = {0, 0, 0, 0}, LRTB _Padding = {0, 0, 0, 0});

    ecs::Entity CreateText(ecs::Entity _Parent, const std::string &_Text, Font &_Font,
                           float _FontSize = 24, float _Spacing = 5,
                           Color _Color = {255, 255, 255, 255},
                           Size _Size = Size{SizeType::FILL, SizeType::FILL},
                           LRTB _Margin = {0, 0, 0, 0}, LRTB _Padding = {0, 0, 0, 0});

    // TODO: Think how to make this better!
    ecs::Entity CreateButton(ecs::Entity _Parent, std::function<void()> _OnClick, Texture &_Texture,
                             LRTB _Patch = {0, 0, 0, 0}, Color _Color = {255, 255, 255, 255},
                             Size _Size = Size{SizeType::FILL, SizeType::FILL},
                             LRTB _Margin = {0, 0, 0, 0}, LRTB _Padding = {0, 0, 0, 0});

    ecs::Entity CreateLabeledButton(ecs::Entity _Parent, std::function<void()> _OnClick,
                                    Texture &_Texture, const std::string &_Text, Font &_Font,
                                    float _FontSize = 24, float _Spacing = 5,
                                    LRTB _Patch = {0, 0, 0, 0}, Color _Color = {255, 255, 255, 255},
                                    Size _Size = Size{SizeType::FILL, SizeType::FILL},
                                    LRTB _Margin = {0, 0, 0, 0}, LRTB _Padding = {0, 0, 0, 0});

private:
    ecs::Registry &m_registry;
    void SetParent(ecs::Entity _Child, ecs::Entity _Parent);
};
} // namespace feyerverx
