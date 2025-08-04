// Copyright 2025 Valentin Namleev

#pragma once

#include <iostream>
#include <string>

#include <Vector2.hpp>
#include <Window.hpp>

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
    InitUI();
  }

  ~Engine() { delete m_Window; }

  void Run() {
    while (!m_Window->ShouldClose()) {
      Update();
      Render();
    }
  }

private:
  void InitWindow(const std::string &_WindowTitle) {
    unsigned int windowFlags =
        FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI | FLAG_WINDOW_RESIZABLE;
    m_Window =
        new raylib::Window(m_Size.x, m_Size.y, _WindowTitle, windowFlags);
  }

  void InitUI() {
    m_UIRoot = turnip::Node::Create("root", nullptr);
    m_UIRoot->SetRect(Rectangle{0, 0, m_Size.x, m_Size.y});
    m_UIRoot->SetPadding({15, 15, 15, 15});

    auto mainStack =
        Stack::Create("mainStack", m_UIRoot, Stack::StackType::HORIZONTAL);
    mainStack->SetSpacing(15);
    mainStack->SetPadding(LRTB{20, 20, 20, 20});

    Image::Create("img", mainStack,
                  ImageInfo{LoadTexture("bean.png"), {0, 0, 0, 0}},
                  turnip::Size{
                      SizeType::START,
                      SizeType::FILL,
                      200,
                  })
        ->SetMargin({15, 15, 15, 15});

    // Image::Create("img2", mainStack,
    //               ImageInfo{LoadTexture("crate.png"), {0, 0, 0, 0}});
    // Image::Create("img3", mainStack,
    //               ImageInfo{LoadTexture("turnip.png"), {0, 0, 0, 0}});
  }

  void Update() {
    m_UIRoot->Traverse([](auto _Node) { _Node->ProcessLayout(); });
  }

  void Render() {
    BeginDrawing();
    m_Window->ClearBackground(WHITE);

    m_UIRoot->Traverse([](auto _Node) { _Node->Render(); });
    m_UIRoot->Traverse([](auto _Node) { _Node->RenderDebug(); });

    EndDrawing();
  }

private:
  Vector2 m_Size;
  raylib::Window *m_Window;
  std::shared_ptr<Node> m_UIRoot;
};
} // namespace turnip
