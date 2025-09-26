// Copyright 2025 Valentin Namleev

#include "turnip/turnip.hpp"

namespace turnip {
Turnip::Turnip(const char *_title, int _width, int _height) : App(_title, _width, _height) {}

void Turnip::SetScreenTexture(const char *_filepath) {
    m_ScreenTexture = m_assetLoader.LoadTexture(_filepath);
}

void Turnip::Update() {
    m_renderer.RenderTexture(m_ScreenTexture, Rectangle{0, 0, 400, 300});
    m_renderer.RenderTexture(m_ScreenTexture, Rectangle{400, 300, 400, 300});
}

void Turnip::ProcessEvent(const SDL_Event &_event) {
    if (_event.type == SDL_EVENT_KEY_DOWN) {
        if (_event.key.key == SDLK_RIGHT) {
            m_ScreenTexture = m_assetLoader.LoadTexture("resources/textures/bush.png");
        }
    }
}
} // namespace turnips
