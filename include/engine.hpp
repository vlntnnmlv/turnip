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
#include "./ecs/systems/UISystem.hpp"
#include "./ecs/systems/renderSystem.hpp"
#include "./image.hpp"
#include "./lrtb.hpp"
#include "./node.hpp"
#include "./stack.hpp"

namespace turnip {
class Engine {
public:
  Engine(float _WindowWidth, float _WindowHeight,
         const std::string &_WindowTitle) {
    m_Size = {_WindowWidth, _WindowHeight};

    InitWindow(_WindowTitle);
    InitSystems();
    InitUI();
  }

  ~Engine() { delete m_Window; }

  void Run() {
    SetTargetFPS(120);

    while (!m_Window->ShouldClose()) {
      Update();
    }
  }

private:
  void InitWindow(const std::string &_WindowTitle) {
    unsigned int windowFlags =
        FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI | FLAG_WINDOW_RESIZABLE;
    m_Window =
        new raylib::Window(m_Size.x, m_Size.y, _WindowTitle, windowFlags);
  }

  void InitUI() { CreateRootNode(); }

  void InitSystems() {
    m_UISystem.Init(m_Size);
    m_RenderSystem.Init(m_Window);
  }

  void Update() {
    float deltaTime = GetFrameTime();

    m_UISystem.Update(deltaTime);
    m_RenderSystem.Update(deltaTime);
  }

  void CreateRootNode() {
    m_UIRoot = m_Registry.CreateEntity();

    m_Registry.AddComponent<ecs::TransformComponent>(m_UIRoot);
    m_Registry.AddComponent<ecs::ChildrenComponent>(m_UIRoot);
    m_Registry.AddComponent<ecs::LayoutComponent>(
        m_UIRoot, turnip::Size{SizeType::FILL, SizeType::FILL},
        LRTB{0, 0, 0, 0}, LRTB{20, 20, 20, 20});

    auto p = m_Registry.CreateEntity();
    m_Registry.AddComponent<ecs::TransformComponent>(p);
    m_Registry.AddComponent<ecs::ParentComponent>(p, m_UIRoot);

    m_Registry.GetComponent<ecs::ChildrenComponent>(m_UIRoot)
        ->children.push_back(p);

    m_Registry.AddComponent<ecs::LayoutComponent>(
        p, turnip::Size{SizeType::FILL, SizeType::FILL}, LRTB{0, 0, 0, 0},
        LRTB{0, 0, 0, 0});
    m_Registry.AddComponent<ecs::SpriteComponent>(
        p, LoadTexture("./resources/textures/crate.png"));
  }

private:
  Vector2 m_Size;
  raylib::Window *m_Window;

  ecs::EntityID m_UIRoot;

  ecs::Registry m_Registry;

  ecs::UISystem m_UISystem{m_Registry};
  ecs::RenderSystem m_RenderSystem{m_Registry};
};
} // namespace turnip
