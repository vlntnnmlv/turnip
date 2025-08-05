// Copyright 2025 Valentin Namleev

#pragma once

#include <iostream>
#include <string>

#include <Vector2.hpp>
#include <Window.hpp>

#include "./ecs/components/childrenComponent.hpp"
#include "./ecs/components/layoutComponent.hpp"
#include "./ecs/components/spriteComponent.hpp"
#include "./ecs/components/transformComponent.hpp"
#include "./ecs/registry.hpp"
#include "./ecs/systems/renderSystem.hpp"
#include "./ecs/systems/uiSystem.hpp"
#include "./image.hpp"
#include "./node.hpp"
#include "./stack.hpp"

namespace turnip {
class Engine {
public:
    Engine(float _WindowWidth, float _WindowHeight, const std::string &_WindowTitle)
        : m_Size{_WindowWidth, _WindowHeight},
          m_Window(
              std::make_unique<raylib::Window>(m_Size.x, m_Size.y, _WindowTitle, m_WindowFlags)),
          m_UISystem(m_Registry, m_Size), m_RenderSystem(m_Registry, m_Window) {

        InitUI();
    }

    ~Engine() = default;

    void Run() {
        SetTargetFPS(120);

        while (!m_Window->ShouldClose()) {
            Update();
        }
    }

private:
    unsigned int m_WindowFlags = FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI | FLAG_WINDOW_RESIZABLE;

    void InitUI() { CreateRootNode(); }

    void Update() {
        float deltaTime = GetFrameTime();

        m_UISystem.Update(deltaTime);
        m_RenderSystem.Update(deltaTime);
    }

    Color GetMonoColor(float _Phase) {
        return Color{(unsigned char)(255.0f * _Phase), (unsigned char)(255.0f * _Phase),
                     (unsigned char)(255.0f * _Phase), 255};
    }

    void CreateRootNode() {
        m_RenderSystem.SetBackgroundColor(Color{86, 86, 86, 255});
        m_UIRoot = m_Registry.CreateEntity();

        m_Registry.AddComponent<ecs::TransformComponent>(m_UIRoot);
        m_Registry.AddComponent<ecs::ChildrenComponent>(m_UIRoot);
        m_Registry.AddComponent<ecs::LayoutComponent>(m_UIRoot,
                                                      turnip::Size{SizeType::FILL, SizeType::FILL},
                                                      LRTB{0, 0, 0, 0}, LRTB{10, 10, 10, 10});

        auto stack = CreateStack(m_UIRoot, 0);

        // Texture2D texture = LoadTexture("./resources/textures/crate.png");

        RenderTexture2D t = LoadRenderTexture(1, 1);
        BeginTextureMode(t);
        DrawPixel(0, 0, WHITE);
        EndTextureMode();
        SetTextureFilter(t.texture, TextureFilter::TEXTURE_FILTER_POINT);
        Texture2D texture = t.texture;

        for (int i = 0; i < 32; ++i) {
            CreateImage(stack, texture, GetMonoColor(1 - i / 32.0f));
        }
    }

    ecs::EntityID CreateStack(ecs::EntityID _Parent, float _Spacing) {
        ecs::EntityID stack = m_Registry.CreateEntity();

        m_Registry.AddComponent<ecs::TransformComponent>(stack);
        m_Registry.AddComponent<ecs::ParentComponent>(stack, _Parent);

        m_Registry.GetComponent<ecs::ChildrenComponent>(_Parent)->children.push_back(stack);

        m_Registry.AddComponent<ecs::ChildrenComponent>(stack);
        m_Registry.AddComponent<ecs::LayoutComponent>(stack,
                                                      turnip::Size{SizeType::FILL, SizeType::FILL},
                                                      LRTB{0, 0, 0, 0}, LRTB{0, 0, 0, 0});
        m_Registry.AddComponent<ecs::StackComponent>(stack, ecs::StackType::HORIZONTAL,
                                                     ecs::StackContentType::CENTER, _Spacing);

        return stack;
    }

    ecs::EntityID CreateImage(ecs::EntityID _Parent, Texture2D _Texture, Color _Color) {
        ecs::EntityID image = m_Registry.CreateEntity();

        m_Registry.AddComponent<ecs::TransformComponent>(image);
        m_Registry.AddComponent<ecs::ParentComponent>(image, _Parent);

        m_Registry.GetComponent<ecs::ChildrenComponent>(_Parent)->children.push_back(image);

        m_Registry.AddComponent<ecs::LayoutComponent>(
            image, turnip::Size{SizeType::FILL, SizeType::FILL}, LRTB{0, 0, 0, 0});
        m_Registry.AddComponent<ecs::SpriteComponent>(image, _Texture);
        m_Registry.AddComponent<ecs::ColorComponent>(image, _Color);

        return image;
    }

private:
    Vector2 m_Size;
    std::unique_ptr<raylib::Window> m_Window;

    ecs::EntityID m_UIRoot;

    ecs::Registry m_Registry;

    ecs::UISystem m_UISystem;
    ecs::RenderSystem m_RenderSystem;
};
} // namespace turnip
