// Copyright 2025 Valentin Namleev

#pragma once

#include <Image.hpp>
#include <Texture.hpp>
#include <TextureUnmanaged.hpp>
#include <format>
#include <iostream>
#include <unordered_map>

namespace turnip {
class ResourcesManager {
public:
    ResourcesManager() = default;
    ~ResourcesManager() = default;

    raylib::Texture2D &GetTexture(const std::string &_Name) {
        if (!m_Textures.contains(_Name)) {
            const std::string &pathToPNG = std::format("./resources/textures/{}.png", _Name);
            m_Textures[_Name] = std::make_unique<raylib::Texture2D>(pathToPNG);
        }

        return *m_Textures[_Name];
    }

    raylib::Texture2D &GetDefaultTexture() {
        if (!m_Textures.contains(DEFAULT_TEXTURE_NAME)) {
            CreateDefaultTexture();
        }

        return *m_Textures[DEFAULT_TEXTURE_NAME];
    }

    raylib::Texture2D &GetFrameTexture(size_t _Thickness) {
        if (!m_FrameTextures.contains(_Thickness)) {
            CreateFrameTexture(_Thickness);
        }

        return *m_FrameTextures[_Thickness];
    }

    raylib::Font &GetFont(const std::string &_Name) {
        if (!m_Fonts.contains(_Name)) {
            const std::string &pathToTTF = std::format("./resources/fonts/{}.ttf", _Name);
            m_Fonts[_Name] = std::make_unique<raylib::Font>(pathToTTF, 128);
        }

        return *m_Fonts[_Name];
    }

private:
    static constexpr const char *DEFAULT_TEXTURE_NAME = "__default_texture";

    std::unordered_map<std::string, std::unique_ptr<raylib::Texture2D>> m_Textures;
    std::unordered_map<size_t, std::unique_ptr<raylib::Texture2D>> m_FrameTextures;
    std::unordered_map<std::string, std::unique_ptr<raylib::Font>> m_Fonts;

    void CreateDefaultTexture() {
        RenderTexture2D t = LoadRenderTexture(1, 1);
        BeginTextureMode(t);
        DrawPixel(0, 0, WHITE);
        EndTextureMode();
        SetTextureFilter(t.texture, TextureFilter::TEXTURE_FILTER_POINT);

        // TODO:
        // Loading RednerTexture2D with raylib-cpp gives a Raylib warning:
        // "UNSUPPORTED (log once): POSSIBLE ISSUE: unit 0 GLD_TEXTURE_INDEX_2D is unloadable and
        // bound to sampler type (Float) - using zero texture because texture unloadable"
        // Figure out why.

        // raylib::RenderTexture2D t = raylib::RenderTexture2D::Load(1, 1);
        // t.BeginMode();
        // DrawPixel(0, 0, WHITE);
        // t.EndMode();
        // SetTextureFilter(t.texture, TextureFilter::TEXTURE_FILTER_POINT);

        m_Textures[DEFAULT_TEXTURE_NAME] = std::make_unique<raylib::Texture2D>(t.texture);
    }

    void CreateFrameTexture(size_t _Thickness) {
        size_t size = _Thickness * 2 + 1;
        RenderTexture2D t = LoadRenderTexture(size, size);
        BeginTextureMode(t);

        for (size_t i = 0; i < size * size; i++) {
            size_t x = i % size;
            size_t y = i / size;

            if (x < _Thickness || y < _Thickness || x > size - _Thickness || y > size - _Thickness)
                DrawPixel(x, y, WHITE);
        }

        EndTextureMode();
        SetTextureFilter(t.texture, TextureFilter::TEXTURE_FILTER_POINT);

        m_FrameTextures[_Thickness] = std::make_unique<raylib::Texture2D>(t.texture);
    }
};
} // namespace turnip
