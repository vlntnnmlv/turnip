// Copyright 2025 Valentin Namleev

#pragma once

#include <Texture.hpp>

#include "./lrtb.hpp"

namespace turnip {
struct ImageInfo {
  Texture2D texture;
  LRTB patch;
};
} // namespace turnip
