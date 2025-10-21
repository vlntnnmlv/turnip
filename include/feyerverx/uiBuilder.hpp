// Copyright 2025 Valentin Namleev

#pragma once

#include <functional>

#include "feyerverx/ecs/components/childrenComponent.hpp"
#include "feyerverx/ecs/components/colorComponent.hpp"
#include "feyerverx/ecs/components/layoutComponent.hpp"
#include "feyerverx/ecs/components/renderTransformComponent.hpp"
#include "feyerverx/ecs/components/spriteComponent.hpp"
#include "feyerverx/ecs/components/stackComponent.hpp"
#include "feyerverx/ecs/entity.hpp"
#include "feyerverx/ecs/registry.hpp"

#include "feyerverx/font.hpp"

namespace feyerverx {
class UIBuilder {
public:
    explicit UIBuilder(std::shared_ptr<ecs::Registry> registry);

    UIBuilder(UIBuilder &&) noexcept = default;
    UIBuilder &operator=(UIBuilder &&) noexcept = default;

    UIBuilder(const UIBuilder &) = delete;
    UIBuilder &operator=(const UIBuilder &) = delete;

    ecs::Entity node(ecs::Entity parent, Size size = Size{SizeType::FILL, SizeType::FILL},
                     RectangleOffset margin = {0, 0, 0, 0}, RectangleOffset padding = {0, 0, 0, 0});

    ecs::Entity root(RectangleOffset padding = {0, 0, 0, 0});

    ecs::Entity image(ecs::Entity parent, Texture &texture, RectangleOffset _Patch = {0, 0, 0, 0},
                      Color color = {255, 255, 255, 255},
                      Size _Size = Size{SizeType::FILL, SizeType::FILL},
                      RectangleOffset _Margin = {0, 0, 0, 0}, RectangleOffset _Padding = {0, 0, 0, 0});

    ecs::Entity stack(ecs::Entity _Parent, ecs::StackType _StackType,
                      ecs::StackContentType _StackContentType = ecs::StackContentType::CENTER,
                      float _Spacing = 0, Size _Size = Size{SizeType::FILL, SizeType::FILL},
                      RectangleOffset _Margin = {0, 0, 0, 0}, RectangleOffset _Padding = {0, 0, 0, 0});

    ecs::Entity text(ecs::Entity _Parent, const std::string &_Text, Font &_Font,
                     float _FontSize = 24, float _Spacing = 5, Color _Color = {255, 255, 255, 255},
                     Size _Size = Size{SizeType::FILL, SizeType::FILL}, RectangleOffset _Margin = {0, 0, 0, 0},
                     RectangleOffset _Padding = {0, 0, 0, 0});

    // TODO: Think how to make this better!
    ecs::Entity button(ecs::Entity _Parent, std::function<void()> _OnClick, Texture &_Texture,
                       RectangleOffset _Patch = {0, 0, 0, 0}, Color _Color = {255, 255, 255, 255},
                       Size _Size = Size{SizeType::FILL, SizeType::FILL},
                       RectangleOffset _Margin = {0, 0, 0, 0}, RectangleOffset _Padding = {0, 0, 0, 0});

    ecs::Entity labeledButton(ecs::Entity _Parent, std::function<void()> _OnClick,
                              Texture &_Texture, const std::string &_Text, Font &_Font,
                              float _FontSize = 24, float _Spacing = 5, RectangleOffset _Patch = {0, 0, 0, 0},
                              Color _Color = {255, 255, 255, 255},
                              Size _Size = Size{SizeType::FILL, SizeType::FILL},
                              RectangleOffset _Margin = {0, 0, 0, 0}, RectangleOffset _Padding = {0, 0, 0, 0});

private:
    std::shared_ptr<ecs::Registry> m_registry;
    void setParent(ecs::Entity child, ecs::Entity parent);
};
} // namespace feyerverx
