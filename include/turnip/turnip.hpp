// Copyright 2025 Valentin Namleev

#pragma once

#include "turnip/app.hpp"
#include "turnip/assetLoader.hpp"
#include "turnip/ecs/registry.hpp"
#include "turnip/renderer.hpp"

namespace turnip {
class Turnip : public App {
public:
    Turnip(const char *title, int width, int height);
    virtual ~Turnip() override = default;

    void SetScreenTexture(const char *filepath);

private:
    void Update() override;
    void ProcessEvent(const SDL_Event &event) override;

private:
    bgfx::TextureHandle m_ScreenTexture;
    AssetLoader m_assetLoader{};
    Renderer m_renderer{};

    ecs::Registry m_registry{};
};
} // namespace turnip
