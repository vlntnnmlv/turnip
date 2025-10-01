// Copyright 2025 Valentin Namleev

#pragma once

#include <bgfx/bgfx.h>

#include "feyerverx/assetManager.hpp"

namespace feyerverx {
class Texture {
public:
    ~Texture() = default;
    Texture(const bgfx::TextureHandle &textureHandle);
    bgfx::TextureHandle handle() const;

private:
private:
    bgfx::TextureHandle m_textureHandle;
};
} // namespace feyerverx
