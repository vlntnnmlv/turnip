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

// TODO: Update systems only when data changes (dirtyComponent)
// TODO: Add audio system
// TODO: Add a build flow for Linux, MacOS and Windows

namespace feyerverx {
class Engine {
public:
    Engine(float _WindowWidth, float _WindowHeight, const std::string &_WindowTitle);
    ~Engine() = default;

    UISceneBuilder &UISceneBuilder();
    ResourcesManager &ResourcesManager();

    ecs::Registry &Registry();
    ecs::RenderSystem &RenderSystem();

    void AddUpdateStep(ecs::UpdateCallback _UpdateCallback);
    void ShowFPS(bool _Value);

    void Run();

private:
    unsigned int m_WindowFlags = FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI | FLAG_WINDOW_RESIZABLE;

    void Update();

private:
    Vector2 m_Size;
    std::unique_ptr<raylib::Window> m_Window;

    ecs::Registry m_Registry;

    // TODO:
    // std::unordered_map<std::type_index, ecs::ISystem> m_Systems;

    ecs::UISystem m_UISystem;
    ecs::RenderSystem m_RenderSystem;
    ecs::InputSystem m_InputSystem;
    ecs::UpdateSystem m_UpdateSystem;

    events::EventQueue m_EventQueue;

    feyerverx::ResourcesManager m_ResourcesManager;

    feyerverx::UISceneBuilder m_UISceneBuilder;
};
} // namespace feyerverx
