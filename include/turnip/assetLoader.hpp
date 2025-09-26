// Copyright 2025 Valentin Namleev

#pragma once

#include <bgfx/bgfx.h>
#include <bimg/decode.h>
#include <bx/readerwriter.h>

namespace turnip {
class AssetLoader {
public:
    bgfx::TextureHandle LoadTexture(const char *_filePath);
    bgfx::ShaderHandle LoadShader(const char *_filePath);

private:
    static bx::DefaultAllocator s_allocator;
};
} // namespace turnip
