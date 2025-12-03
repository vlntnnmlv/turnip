#include <format>

#include <bgfx/bgfx.h>

#include "feyerverx/assetManager.hpp"

#include <filesystem>
#include <print>
#include <ranges>

namespace feyerverx {
AssetManager::~AssetManager() {
    for (auto &texture : m_assets | std::views::values) {
        bgfx::destroy(texture.handle());
    }
}

Texture AssetManager::getTexture(const std::string &textureName) {
    if (m_assets.contains(textureName))
        return m_assets.at(textureName);

    // TODO: Bake all assets the same as shaders
    const std::filesystem::path texturePath =
        std::filesystem::absolute(std::format("../../../resources/textures/{}.png", textureName));

    bgfx::TextureHandle textureHandle = AssetLoader::loadTexture(texturePath);

    m_assets.emplace(textureName, textureHandle);
    return m_assets.at(textureName);
}
}