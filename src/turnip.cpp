// Copyright 2025 Valentin Namleev

#include "turnip.hpp"

namespace turnip {
static bgfx::TextureHandle texture;

Turnip::Turnip(const char *_title, int _width, int _height) : App(_title, _width, _height) {
    texture = m_assetManager.LoadTexture("resources/textures/bean.png");
}
Turnip::~Turnip() {}
void Turnip::Update(bgfx::ProgramHandle _program) { m_renderer.RenderTexture(texture, _program); }
} // namespace turnip
