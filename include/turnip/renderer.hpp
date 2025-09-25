// Copyright 2025 Valentin Namleev

#pragma once

#include <bgfx/bgfx.h>

namespace turnip {
struct Vertex {
    float X, Y, Z;
    float U, V;

    static void InitLayout(bgfx::VertexLayout &_layout) {
        _layout.begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
            .end();
    }
};

class Renderer {
public:
    void RenderTexture(bgfx::TextureHandle _textureHandle, bgfx::ProgramHandle _program);

private:
};
} // namespace turnip
