// Copyright 2025 Valentin Namleev

#pragma once

#include <string>

#include <bgfx/bgfx.h>
#include <bimg/decode.h>
#include <bx/readerwriter.h>

namespace feyerverx {
class AssetLoader {
public:
    bgfx::TextureHandle loadTexture(const std::string &filename);
    bgfx::ShaderHandle loadShader(const std::string &filename);
};
} // namespace feyerverx
