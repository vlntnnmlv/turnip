#include <utility>

#include "feyerverx/texture.hpp"

namespace feyerverx {
Texture::Texture(const bgfx::TextureHandle &textureHandle)
    : m_textureHandle(std::move(textureHandle)) {}

bgfx::TextureHandle Texture::handle() const { return m_textureHandle; }
}