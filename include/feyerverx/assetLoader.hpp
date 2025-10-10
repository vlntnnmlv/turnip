// Copyright 2025 Valentin Namleev

#pragma once

#include <string>

#include <bgfx/bgfx.h>

namespace feyerverx {
class AssetLoader {
public:
    static bgfx::TextureHandle loadTexture(const std::string &filepath);
    static bgfx::ShaderHandle loadShader(const std::string &filepath);
};
} // namespace feyerverx
