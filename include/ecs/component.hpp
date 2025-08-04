// Copyright 2025 Valentin Namleev

#pragma once

namespace turnip::ecs {
struct IComponent {
  virtual ~IComponent() = default;
};
} // namespace turnip::ecs
