// Copyright 2025 Valentin Namleev

#pragma once

#include <bgfx/bgfx.h>

#include "feyerverx/logger.hpp"
#include "feyerverx/rectangle.hpp"
#include "feyerverx/texture.hpp"

namespace feyerverx {
template <typename T>
concept IsHandle = requires(T handle) {
    handle.idx; // Requires the existence of a member named 'foo'
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

    void init();
    void renderTexture(const Texture &texture, const Rectangle &rectangle);

private:
    template <IsHandle T> void destroyHandle(T handle) {
        if (bgfx::isValid(handle)) {
            bgfx::destroy(handle);
        }
    }

private:
    bgfx::VertexLayout m_layout;

    bgfx::ProgramHandle m_program BGFX_INVALID_HANDLE;
    bgfx::VertexBufferHandle m_vertexBuffer BGFX_INVALID_HANDLE;
    bgfx::IndexBufferHandle m_trianglesBuffer BGFX_INVALID_HANDLE;
    bgfx::UniformHandle m_textureSamplerUniform BGFX_INVALID_HANDLE;
};
} // namespace feyerverx
