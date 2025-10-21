// Copyright 2025 Valentin Namleev

#pragma once

#include <bgfx/bgfx.h>

#include "feyerverx/logger.hpp"
#include "feyerverx/rectangle.hpp"
#include "feyerverx/texture.hpp"

namespace feyerverx {
template <typename T>
concept IsHandle = requires(T handle) { handle.idx; };

struct Vertex {
    float x;
    float y;
    float z;
    float u;
    float v;

    static void InitLayout(bgfx::VertexLayout &layout) {
        layout.begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
            .end();
    }
};

class Renderer {
public:
    static std::unique_ptr<Renderer> create();

    Renderer(const Renderer &) = delete;
    Renderer &operator=(const Renderer &) = delete;
    Renderer(Renderer &&) noexcept;
    Renderer &operator=(Renderer &&) = delete;

    ~Renderer();

    void renderTexture(const Texture &texture, const Rectangle &rectangle, uint16_t viewID);

private:
    Renderer(const bgfx::VertexLayout &layout, const bgfx::VertexBufferHandle &vertexBuffer,
             const bgfx::IndexBufferHandle &trianglesBuffer, const bgfx::ProgramHandle &program,
             const bgfx::UniformHandle &textureSamplerUniform);

    template <IsHandle T> void destroyHandle(T handle) {
        Logger::instance().log(LogLevel::Debug, "Trying to destroy render handle {}", handle.idx);
        if (bgfx::isValid(handle)) {
            bgfx::destroy(handle);
            Logger::instance().log(LogLevel::Debug, "Destroyed render handle {}", handle.idx);
        }
    }

    bgfx::VertexLayout m_layout;
    bgfx::VertexBufferHandle m_vertexBuffer{bgfx::kInvalidHandle};
    bgfx::IndexBufferHandle m_trianglesBuffer{bgfx::kInvalidHandle};
    bgfx::ProgramHandle m_program{bgfx::kInvalidHandle};
    bgfx::UniformHandle m_textureSamplerUniform{bgfx::kInvalidHandle};
};
} // namespace feyerverx
