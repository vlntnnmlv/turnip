// Copyright 2025 Valentin Namleev

#pragma once

#include "../../axis.hpp"
#include "../../rectangleUtils.hpp"
#include "../components/layoutComponent.hpp"
#include "../components/parentComponent.hpp"
#include "../components/transformComponent.hpp"
#include "../registry.hpp"
#include "../system.hpp"

namespace turnip::ecs {
class UISystem : protected ISystem {
public:
  UISystem(Registry &_Registry) : ISystem(_Registry) {}

  void Update(float _DeltaTime) override { ProcessLayout(); }
  void Init(Vector2 _Size) { m_Size = _Size; }

private:
  Vector2 m_Size;
  void ProcessLayout() {
    auto roots = FindRoots();
    for (auto root : roots) {

      auto transform = m_Registry.GetComponent<TransformComponent>(root);
      transform->rect.width = m_Size.x;
      transform->rect.height = m_Size.y;

      Measure(root);
      Arrange(root);
      PlaceInWorld(root);
    }
  }

  std::vector<EntityID> FindRoots() {
    std::vector<EntityID> roots;

    for (const auto &e : m_Registry.With<TransformComponent>()) {
      if (!m_Registry.GetComponent<ParentComponent>(e)) {
        roots.push_back(e);
      }
    }

    return roots;
  }

  void Measure(EntityID _EntityID) {
    ChildrenComponent *childrenComponent =
        m_Registry.GetComponent<ChildrenComponent>(_EntityID);

    TransformComponent *transformComponent =
        m_Registry.GetComponent<TransformComponent>(_EntityID);

    // std::cout << _EntityID << " <> " << transformComponent->rect.width << " "
    //           << transformComponent->rect.height << " "
    //           << transformComponent->rect.x << " " <<
    //           transformComponent->rect.y
    //           << "\n";

    if (!childrenComponent) {
      return;
    }

    LayoutComponent *layoutComponent =
        m_Registry.GetComponent<LayoutComponent>(_EntityID);

    for (const auto &child : childrenComponent->children) {

      LayoutComponent *childLayoutComponent =
          m_Registry.GetComponent<LayoutComponent>(child);
      TransformComponent *childTransformComponent =
          m_Registry.GetComponent<TransformComponent>(child);

      for (Axis axis : {Axis::VERTICAL, Axis::HORIZONTAL}) {
        float availableSpace =
            AxisHelper::GetRectSize(transformComponent->rect, axis) -
            AxisHelper::GetPadding(layoutComponent->padding, axis, true) -
            AxisHelper::GetPadding(layoutComponent->padding, axis, false) -
            AxisHelper::GetPadding(childLayoutComponent->margin, axis, true) -
            AxisHelper::GetPadding(childLayoutComponent->margin, axis, false);

        switch (
            AxisHelper::GetLayoutSizeType(childLayoutComponent->size, axis)) {
        case turnip::SizeType::FILL:
          AxisHelper::GetRectSize(childTransformComponent->rect, axis) =
              availableSpace;
          break;
        case turnip::SizeType::START:
        case turnip::SizeType::END:
        case turnip::SizeType::CENTER:
        case turnip::SizeType::ABSOLUTE:
          AxisHelper::GetRectSize(childTransformComponent->rect, axis) =
              AxisHelper::GetLayoutSize(childLayoutComponent->size, axis);
          break;
        }
      }
    }

    for (const auto &child : childrenComponent->children) {
      Measure(child);
    }
  }

  void Arrange(EntityID _EntityID) {
    ChildrenComponent *childrenComponent =
        m_Registry.GetComponent<ChildrenComponent>(_EntityID);

    if (!childrenComponent)
      return;

    LayoutComponent *layoutComponent =
        m_Registry.GetComponent<LayoutComponent>(_EntityID);
    TransformComponent *transformComponent =
        m_Registry.GetComponent<TransformComponent>(_EntityID);

    for (const auto &child : childrenComponent->children) {
      LayoutComponent *childLayoutComponent =
          m_Registry.GetComponent<LayoutComponent>(child);
      TransformComponent *childTransformComponent =
          m_Registry.GetComponent<TransformComponent>(child);

      for (Axis axis : {Axis::HORIZONTAL, Axis::VERTICAL}) {
        float &pos =
            AxisHelper::GetRectPosition(childTransformComponent->rect, axis);

        switch (
            AxisHelper::GetLayoutSizeType(childLayoutComponent->size, axis)) {
        case turnip::SizeType::FILL:
        case turnip::SizeType::START:
          pos = AxisHelper::GetPadding(layoutComponent->padding, axis, true) +
                AxisHelper::GetMargin(childLayoutComponent->margin, axis, true);
          break;
        case turnip::SizeType::END:
          pos =
              AxisHelper::GetRectSize(transformComponent->rect, axis) -
              AxisHelper::GetPadding(layoutComponent->padding, axis, false) -
              AxisHelper::GetMargin(childLayoutComponent->margin, axis, false) -
              AxisHelper::GetRectSize(childTransformComponent->rect, axis);
          break;
        case turnip::SizeType::CENTER:
          pos = AxisHelper::GetRectPosition(transformComponent->rect, axis) +
                AxisHelper::GetRectSize(transformComponent->rect, axis) / 2.0f -
                AxisHelper::GetRectSize(childTransformComponent->rect, axis) /
                    2.0f;
          break;
        case turnip::SizeType::ABSOLUTE:
          pos = (axis == Axis::HORIZONTAL) ? childLayoutComponent->size.x
                                           : childLayoutComponent->size.y;
          break;
        }
      }
    }

    for (const auto &child : childrenComponent->children) {
      Arrange(child);
    }
  }

  void PlaceInWorld(EntityID _EntityID) {
    TransformComponent *transformComponent =
        m_Registry.GetComponent<TransformComponent>(_EntityID);

    ParentComponent *parentComponent =
        m_Registry.GetComponent<ParentComponent>(_EntityID);

    ChildrenComponent *childrenComponent =
        m_Registry.GetComponent<ChildrenComponent>(_EntityID);

    if (!parentComponent)
      transformComponent->worldRect = transformComponent->rect;
    else {
      TransformComponent *parentTransformComponent =
          m_Registry.GetComponent<TransformComponent>(parentComponent->parent);
      transformComponent->worldRect =
          RectangleUtils::Move(transformComponent->rect,
                               Vector2{parentTransformComponent->worldRect.x,
                                       parentTransformComponent->worldRect.y});
    }

    if (!childrenComponent)
      return;

    for (const auto &child : childrenComponent->children) {
      PlaceInWorld(child);
    }
  }
};
} // namespace turnip::ecs
