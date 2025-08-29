// Copyright 2025 Valentin Namleev

#pragma once

#include <Font.hpp>
#include <Image.hpp>
#include <Texture.hpp>
#include <TextureUnmanaged.hpp>
#include <cmath>
#include <format>
#include <iostream>
#include <unordered_map>

namespace turnip {
class ResourcesManager {
public:
    ResourcesManager() = default;
    ~ResourcesManager() = default;

    void SetResourcesDirectory(const std::string &_ResourcesDirectory);
    raylib::Texture2D &GetTexture(const std::string &_Name);
    raylib::Texture2D &GetDefaultTexture();
    raylib::Texture2D &GetFrameTexture(size_t _Thickness);
    raylib::Texture2D &GetSmoothCornerTexture(size_t _BorderRadius);
    raylib::Font &GetFont(const std::string &_Name);

private:
    static constexpr const char *DEFAULT_TEXTURE_NAME = "__default_texture";

    std::string m_ResourcesDirectory;

    std::unordered_map<std::string, std::unique_ptr<raylib::Texture2D>> m_Textures;
    std::unordered_map<size_t, std::unique_ptr<raylib::Texture2D>> m_FrameTextures;
    std::unordered_map<size_t, std::unique_ptr<raylib::Texture2D>> m_SmoothCornerTextures;
    std::unordered_map<std::string, std::unique_ptr<raylib::Font>> m_Fonts;

    void CreateDefaultTexture();
    void CreateSmoothCornerTexture(int _BorderRadius);
    void CreateFrameTexture(size_t _Thickness);
};
} // namespace turnip
