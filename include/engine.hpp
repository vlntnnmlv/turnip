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
#include "./ecs/components/updateComponent.hpp"
#include "./ecs/entity.hpp"
#include "./ecs/registry.hpp"
#include "./ecs/systems/inputSystem.hpp"
#include "./ecs/systems/renderSystem.hpp"
#include "./ecs/systems/uiSystem.hpp"
#include "./ecs/systems/updateSystem.hpp"
#include "./resourcesManager.hpp"
#include "./uiSceneBuilder.hpp"

// TODO: Add dirtyComponent, to not update every system every frame

namespace turnip {
class Engine {
public:
    Engine(float _WindowWidth, float _WindowHeight, const std::string &_WindowTitle)
        : m_Size{_WindowWidth, _WindowHeight},
          m_Window(
              std::make_unique<raylib::Window>(m_Size.x, m_Size.y, _WindowTitle, m_WindowFlags)),
          m_InputSystem(m_Registry, m_EventQueue), m_UISystem(m_Registry, m_EventQueue, m_Size),
          m_RenderSystem(m_Registry, m_Window), m_UpdateSystem(m_Registry),
          m_UISceneBuilder(m_Registry) {
        InitUI();
    }

    ~Engine() = default;

    UISceneBuilder &UISceneBuilder() { return m_UISceneBuilder; }
    ResourcesManager &ResourcesManager() { return m_ResourcesManager; }
    ecs::Registry &Registry() { return m_Registry; }

    void AddUpdateStep(std::function<void(float)> _UpdateStep) {
        ecs::EntityID updateStep = m_Registry.CreateEntity();
        m_Registry.AddComponent<ecs::UpdateComponent>(updateStep, _UpdateStep);
    }

    ecs::Entity CreateEntity() { return ecs::Entity(m_Registry); }

    void Run() {
        SetTargetFPS(120);

        while (!m_Window->ShouldClose()) {
            Update();
        }
    }

private:
    unsigned int m_WindowFlags = FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI | FLAG_WINDOW_RESIZABLE;

    void InitUI() { m_RenderSystem.SetBackgroundColor({52, 67, 94}); }

    void Update() {
        float deltaTime = GetFrameTime();

        m_UpdateSystem.Update(deltaTime);
        m_InputSystem.Update(deltaTime);
        m_UISystem.Update(deltaTime);
        m_RenderSystem.Update(deltaTime);
    }

private:
    Vector2 m_Size;
    std::unique_ptr<raylib::Window> m_Window;

    ecs::Registry m_Registry;

    ecs::UISystem m_UISystem;
    ecs::RenderSystem m_RenderSystem;
    ecs::InputSystem m_InputSystem;
    ecs::UpdateSystem m_UpdateSystem;

    events::EventQueue m_EventQueue;

    ecs::EntityID m_UIRoot;

    turnip::ResourcesManager m_ResourcesManager;

    turnip::UISceneBuilder m_UISceneBuilder;
};
} // namespace turnip
