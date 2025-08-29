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
    Engine(float _WindowWidth, float _WindowHeight, const std::string &_WindowTitle,
           const std::string &_ResourcesFolder);

    ~Engine() = default;

    UISceneBuilder &UISceneBuilder();
    ResourcesManager &ResourcesManager();
    ecs::Registry &Registry();

    void AddUpdateStep(std::function<void(float)> _UpdateStep);

    ecs::Entity CreateEntity();

    void Run();

private:
    unsigned int m_WindowFlags = FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI | FLAG_WINDOW_RESIZABLE;

    void InitUI();
    void Update();

private:
    Vector2 m_Size;
    std::unique_ptr<raylib::Window> m_Window;

    ecs::Registry m_Registry;

    ecs::UISystem m_UISystem;
    ecs::RenderSystem m_RenderSystem;
    ecs::InputSystem m_InputSystem;
    ecs::UpdateSystem m_UpdateSystem;

    events::EventQueue m_EventQueue;

    turnip::ResourcesManager m_ResourcesManager;

    turnip::UISceneBuilder m_UISceneBuilder;
};
} // namespace turnip
