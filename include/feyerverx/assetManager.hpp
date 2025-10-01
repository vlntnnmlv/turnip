// Copyright 2025 Valentin Namleev

#pragma once

#include <string>
#include <unordered_map>

#include "feyerverx/assetLoader.hpp"
#include "feyerverx/texture.hpp"

namespace feyerverx {
class Texture;

class AssetManager {
public:
    AssetManager() = default;
    ~AssetManager();
    Texture getTexture(const std::string &textureName);

private:
    AssetLoader m_assetLoader;
    std::unordered_map<std::string, Texture> m_assets{};
};
} // namespace feyerverx
