// Copyright 2025 Valentin Namleev

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <exception>
#include <format>
#include <print>

#include "feyerverx/assetLoader.hpp"

namespace feyerverx {
bgfx::TextureHandle AssetLoader::loadTexture(const std::string &filepath) {
    int width, height, channels;
    stbi_uc *pixels = stbi_load(filepath.c_str(), &width, &height, &channels, 4); // Force RGBA8

    if (!pixels) {
        std::print("Texture failed to load!\n", filepath);
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

    if (!bgfx::isValid(textureHandle)) {
        std::print("Texture failed to load!\n");
    }

    return textureHandle;
}

bgfx::ShaderHandle AssetLoader::loadShader(const std::string &filepath) {
    const char *shaderPath = "???";

    switch (bgfx::getRendererType()) {
    case bgfx::RendererType::Noop:
    case bgfx::RendererType::Direct3D11:
    case bgfx::RendererType::Direct3D12:
        shaderPath = "resources/shaders/dx11/";
        break;
    case bgfx::RendererType::Gnm:
        shaderPath = "resources/shaders/pssl/";
        break;
    case bgfx::RendererType::Metal:
        shaderPath = "resources/shaders/metal";
        break;
    case bgfx::RendererType::OpenGL:
        shaderPath = "resources/shaders/glsl/";
        break;
    case bgfx::RendererType::OpenGLES:
        shaderPath = "resources/shaders/essl/";
        break;
    case bgfx::RendererType::Vulkan:
        shaderPath = "resources/shaders/spirv";
        break;
    }

    const char *p = std::format("examples/simple/{0}", filepath.c_str()).c_str();
    FILE *file = fopen(std::format("examples/simple/{0}", filepath.c_str()).c_str(), "rb");
    if (file == NULL) {
        throw std::runtime_error("Couldn't load shader file at:");
    }

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    const bgfx::Memory *mem = bgfx::alloc(fileSize + 1);
    fread(mem->data, 1, fileSize, file);
    mem->data[mem->size - 1] = '\0';
    fclose(file);

    return bgfx::createShader(mem);
}
}