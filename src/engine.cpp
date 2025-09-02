// Copyright 2025 Valentin Namleev

#include "./turnip/engine.hpp"
// TODO: Add dirtyComponent, to not update every system every frame

namespace turnip {
Engine::Engine(float _WindowWidth, float _WindowHeight, const std::string &_WindowTitle)
    : m_Size{_WindowWidth, _WindowHeight},
      m_Window(std::make_unique<raylib::Window>(m_Size.x, m_Size.y, _WindowTitle, m_WindowFlags)),
      m_UISystem(m_Registry, m_EventQueue, m_Size), m_RenderSystem(m_Registry, m_Window),
      m_InputSystem(m_Registry, m_EventQueue), m_UpdateSystem(m_Registry),
      m_UISceneBuilder(m_Registry) {
    InitUI();
}

UISceneBuilder &Engine::UISceneBuilder() { return m_UISceneBuilder; }
ResourcesManager &Engine::ResourcesManager() { return m_ResourcesManager; }

ecs::Registry &Engine::Registry() { return m_Registry; }
ecs::RenderSystem &Engine::RenderSystem() { return m_RenderSystem; }

void Engine::AddUpdateStep(ecs::UpdateCallback _UpdateStep) {
    ecs::EntityID updateStep = m_Registry.CreateEntity();
    m_Registry.AddComponent<ecs::UpdateComponent>(updateStep, _UpdateStep);
}

ecs::Entity Engine::CreateEntity() { return ecs::Entity(m_Registry); }

void Engine::Run() {
    SetTargetFPS(120);

    while (!m_Window->ShouldClose()) {
        Update();
    }
}

void Engine::InitUI() { m_RenderSystem.SetBackgroundColor({52, 67, 94, 255}); }

void Engine::Update() {
    float deltaTime = GetFrameTime();

    m_UpdateSystem.Update(deltaTime);
    m_InputSystem.Update(deltaTime);
    m_UISystem.Update(deltaTime);
    m_RenderSystem.Update(deltaTime);
}
} // namespace turnip
