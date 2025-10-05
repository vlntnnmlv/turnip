#include <format>

#include <bgfx/bgfx.h>

#include "feyerverx/assetManager.hpp"

namespace feyerverx {
AssetManager::~AssetManager() {
    for (auto it = m_assets.begin(); it != m_assets.end(); it++) {
        bgfx::destroy(it->second.handle());
    }
}

Texture AssetManager::getTexture(const std::string &textureName) {
    if (m_assets.contains(textureName))
        return m_assets.at(textureName);

    bgfx::TextureHandle textureHandle =
        m_assetLoader.loadTexture(std::format("resources/textures/{}.png", textureName));

    m_assets.emplace(textureName, textureHandle);
    return m_assets.at(textureName);
}
}