// Copyright 2025 Valentin Namleev

#pragma once

#include <format>
#include <raylib-cpp.hpp>
#include <unordered_map>

namespace turnip {
class Resources {
public:
    static raylib::Texture2D &GetTexture(const std::string &_Name) {
        if (!m_Textures.contains(_Name)) {
            const std::string &pathToPNG = std::format("./resources/textures/{}.png", _Name);
            m_Textures[_Name] = std::make_unique<raylib::Texture2D>(pathToPNG);
        }

        return *m_Textures[_Name];
    }

    static raylib::Font &GetFont(const std::string &_Name) {
        if (!m_Fonts.contains(_Name)) {
            const std::string &pathToTTF = std::format("./resources/fonts/{}.ttf", _Name);
            m_Fonts[_Name] = std::make_unique<raylib::Font>(pathToTTF);
        }

        return *m_Fonts[_Name];
    }

private:
    static std::unordered_map<std::string, std::unique_ptr<raylib::Texture2D>> m_Textures;
    static std::unordered_map<std::string, std::unique_ptr<raylib::Font>> m_Fonts;
};
} // namespace turnip
