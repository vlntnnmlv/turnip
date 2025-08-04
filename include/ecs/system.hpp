// Copyright 2025 Valentin Namleev

#pragma once

namespace turnip::ecs {
struct ISystem {
  ISystem(Registry &_Registry) : m_Registry(_Registry) {}
  virtual ~ISystem() = default;
  virtual void Update(float _DeltaTime) {}

protected:
  Registry &m_Registry;
};
} // namespace turnip::ecs
