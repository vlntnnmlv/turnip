// Copyright 2025 Valentin Namleev

#include "./turnip/engine.hpp"

namespace turnip {
Engine::Engine(float _WindowWidth, float _WindowHeight, const std::string &_WindowTitle)
    : m_Size{_WindowWidth, _WindowHeight},
      m_Window(std::make_unique<raylib::Window>(m_Size.x, m_Size.y, _WindowTitle, m_WindowFlags)),
      m_UISystem(m_Registry, m_EventQueue, m_Size), m_RenderSystem(m_Registry, m_Window),
      m_InputSystem(m_Registry, m_EventQueue), m_UpdateSystem(m_Registry),
      m_UISceneBuilder(m_Registry) {}

UISceneBuilder &Engine::UISceneBuilder() { return m_UISceneBuilder; }
ResourcesManager &Engine::ResourcesManager() { return m_ResourcesManager; }

ecs::Registry &Engine::Registry() { return m_Registry; }
ecs::RenderSystem &Engine::RenderSystem() { return m_RenderSystem; }

void Engine::AddUpdateStep(ecs::UpdateCallback _UpdateCallback) {
    ecs::Entity updateStep = m_Registry.CreateEntity();
    updateStep.AddComponent<ecs::UpdateComponent>(_UpdateCallback);
}

void Engine::ShowFPS(bool _Value) { m_RenderSystem.ShowFPS(_Value); }

void Engine::Run() {
    SetTargetFPS(120);

    while (!m_Window->ShouldClose()) {
        Update();
    }
}

void Engine::Update() {
    float deltaTime = GetFrameTime();

    m_UpdateSystem.Update(deltaTime);
    m_InputSystem.Update(deltaTime);
    m_UISystem.Update(deltaTime);
    m_RenderSystem.Update(deltaTime);
}
} // namespace turnip
