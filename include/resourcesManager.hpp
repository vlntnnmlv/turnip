// Copyright 2025 Valentin Namleev

#pragma once

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

    raylib::Texture2D &GetSmoothCornerTexture(size_t _BorderRadius) {
        if (!m_SmoothCornerTextures.contains(_BorderRadius)) {
            CreateSmoothCornerTexture(_BorderRadius);
        }

        return *m_SmoothCornerTextures[_BorderRadius];
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
    std::unordered_map<size_t, std::unique_ptr<raylib::Texture2D>> m_SmoothCornerTextures;
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
        // t.EndMode();
        // SetTextureFilter(t.texture, TextureFilter::TEXTURE_FILTER_POINT);

        m_Textures[DEFAULT_TEXTURE_NAME] = std::make_unique<raylib::Texture2D>(t.texture);
    }

    void CreateSmoothCornerTexture(size_t _BorderRadius) {
        size_t size = _BorderRadius * 2 + 1;
        RenderTexture2D t = LoadRenderTexture(size, size);

        BeginTextureMode(t);
        int dx, dy;
        int centerX, centerY;

        for (int x = 0; x < size; x++) {
            for (int y = 0; y < size; y++) {
                if (x == _BorderRadius || y == _BorderRadius) {
                    DrawPixel(x, y, WHITE);
                    continue;
                }

                if (x < _BorderRadius)
                    centerX = _BorderRadius - 1;
                else
                    centerX = _BorderRadius + 1;

                if (y < _BorderRadius)
                    centerY = _BorderRadius - 1;
                else
                    centerY = _BorderRadius + 1;

                dx = std::abs(x - centerX);
                dy = std::abs(y - centerY);

                if (dx * dx + dy * dy < _BorderRadius * _BorderRadius) {
                    DrawPixel(x, y, WHITE);
                }
            }
        }
        EndTextureMode();

        SetTextureFilter(t.texture, TextureFilter::TEXTURE_FILTER_POINT);
        m_SmoothCornerTextures[_BorderRadius] = std::make_unique<raylib::Texture2D>(t.texture);
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
