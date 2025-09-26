// Copyright 2025 Valentin Namleev

#pragma once

#include <bgfx/bgfx.h>

namespace turnip {

struct Rectangle {
    float x;
    float y;
    float width;
    float height;
};

struct Vertex {
    float x;
    float y;
    float z;
    float u;
    float v;

    static void InitLayout(bgfx::VertexLayout &_layout) {
        _layout.begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
            .end();
    }
};

class Renderer {
public:
    Renderer();
    ~Renderer();
    void RenderTexture(bgfx::TextureHandle _textureHandle, const Rectangle &rectangle);

private:
    bgfx::ProgramHandle m_Program;
    bgfx::VertexBufferHandle m_VertexBuffer;
    bgfx::IndexBufferHandle m_TrianglesBuffer;

    bgfx::VertexLayout m_Layout;
    bgfx::UniformHandle m_TextureSamplerUniform;
};
} // namespace turnip
