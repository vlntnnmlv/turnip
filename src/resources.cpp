// Copyright 2025 Valentin Namleev

#include "./resources.hpp"

namespace turnip {
std::unordered_map<std::string, std::unique_ptr<raylib::Texture2D>> Resources::m_Textures;
std::unordered_map<std::string, std::unique_ptr<raylib::Font>> Resources::m_Fonts;
} // namespace turnip
