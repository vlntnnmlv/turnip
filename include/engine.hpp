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
#include "./resourcesManager.hpp"
#include "./uiSceneBuilder.hpp"

namespace turnip {
class Engine {
public:
    Engine(float _WindowWidth, float _WindowHeight, const std::string &_WindowTitle)
        : m_Size{_WindowWidth, _WindowHeight},
          m_Window(
              std::make_unique<raylib::Window>(m_Size.x, m_Size.y, _WindowTitle, m_WindowFlags)),
          m_UISystem(m_Registry, m_Size), m_RenderSystem(m_Registry, m_Window),
          m_UISceneBuilder(m_Registry) {
        InitUI();
    }

    ~Engine() = default;

    UISceneBuilder &UISceneBuilder() { return m_UISceneBuilder; }
    ResourcesManager &ResourcesManager() { return m_ResourcesManager; }

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

    void CreateRootNode() { m_RenderSystem.SetBackgroundColor(WHITE); }

private:
    Vector2 m_Size;
    std::unique_ptr<raylib::Window> m_Window;

    ecs::Registry m_Registry;

    ecs::UISystem m_UISystem;
    ecs::RenderSystem m_RenderSystem;

    ecs::EntityID m_UIRoot;

    turnip::ResourcesManager m_ResourcesManager;

    turnip::UISceneBuilder m_UISceneBuilder;
};
} // namespace turnip
