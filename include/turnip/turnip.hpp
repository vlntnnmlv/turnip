// Copyright 2025 Valentin Namleev

#pragma once

#include "app.hpp"
#include "assetManager.hpp"
#include "renderer.hpp"

namespace turnip {
class Turnip : public App {
public:
    Turnip(const char *_title, int _width, int _height);
    virtual ~Turnip() override;

private:
    void Update(bgfx::ProgramHandle _program) override;

private:
    AssetManager m_assetManager;
    Renderer m_renderer;
};
} // namespace turnip
