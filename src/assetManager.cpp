// Copyright 2025 Valentin Namleev

#include "assetManager.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace turnip {
bgfx::TextureHandle AssetManager::LoadTexture(const char *_filePath) {
    int width, height, channels;
    stbi_uc *pixels = stbi_load(_filePath, &width, &height, &channels, 4); // Force RGBA8

    if (!pixels) {
        // Handle error: texture not found or failed to load
        return BGFX_INVALID_HANDLE;
    }

    const bgfx::Memory *mem =
        bgfx::copy(pixels, width * height * 4); // Copy pixel data to bgfx memory
    stbi_image_free(pixels);                    // Free stb_image's memory

    bgfx::TextureHandle textureHandle = bgfx::createTexture2D(
        (uint16_t)width, (uint16_t)height,
        false, // No mipmaps in this example
        1,     // Number of layers
        bgfx::TextureFormat::RGBA8,
        BGFX_TEXTURE_NONE | BGFX_SAMPLER_POINT, // No special flags, use point sampling
        mem);

    return textureHandle;
}
}