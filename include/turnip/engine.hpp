// Copyright 2025 Valentin Namleev

#pragma once

#include <iostream>
#include <string>

#include <Vector2.hpp>
#include <Window.hpp>

#include "./turnip/ecs/components/childrenComponent.hpp"
#include "./turnip/ecs/components/layoutComponent.hpp"
#include "./turnip/ecs/components/spriteComponent.hpp"
#include "./turnip/ecs/components/transformComponent.hpp"
#include "./turnip/ecs/components/updateComponent.hpp"
#include "./turnip/ecs/entity.hpp"
#include "./turnip/ecs/registry.hpp"
#include "./turnip/ecs/systems/inputSystem.hpp"
#include "./turnip/ecs/systems/renderSystem.hpp"
#include "./turnip/ecs/systems/uiSystem.hpp"
#include "./turnip/ecs/systems/updateSystem.hpp"
#include "./turnip/resourcesManager.hpp"
#include "./turnip/uiSceneBuilder.hpp"

// TODO: Add dirtyComponent, to not update every system every frame

namespace turnip {
class Engine {
public:
    Engine(float _WindowWidth, float _WindowHeight, const std::string &_WindowTitle);
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
