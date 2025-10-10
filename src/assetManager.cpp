#include <format>

#include <bgfx/bgfx.h>

#include "feyerverx/assetManager.hpp"

#include <filesystem>
#include <print>

namespace feyerverx {
AssetManager::~AssetManager() {
    for (auto it = m_assets.begin(); it != m_assets.end(); it++) {
        bgfx::destroy(it->second.handle());
    }
}

Texture AssetManager::getTexture(const std::string &textureName) {
    if (m_assets.contains(textureName))
        return m_assets.at(textureName);

    // TODO: Bake all assets the same as shaders
    const std::filesystem::path texturePath =
        std::filesystem::absolute(std::format("../../../resources/textures/{}.png", textureName));

    std::print("{}", texturePath.c_str());

    bgfx::TextureHandle textureHandle = AssetLoader::loadTexture(texturePath);

    m_assets.emplace(textureName, textureHandle);
    return m_assets.at(textureName);
}
}