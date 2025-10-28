// Copyright 2025 Valentin Namleev

#include <print>

#include <bgfx/embedded_shader.h>
#include <bx/math.h>

#include <essl/quad_fs.sc.bin.h>
#include <essl/quad_vs.sc.bin.h>
#include <glsl/quad_fs.sc.bin.h>
#include <glsl/quad_vs.sc.bin.h>
#include <spirv/quad_fs.sc.bin.h>
#include <spirv/quad_vs.sc.bin.h>
#if defined(_WIN32)
#include <dx11/quad.fs.sc.bin.h>
#include <dx11/quad.vs.sc.bin.h>
#endif //  defined(_WIN32)
#if __APPLE__
#include <metal/quad_fs.sc.bin.h>
#include <metal/quad_vs.sc.bin.h>
#endif // __APPLE__

#include "feyerverx/assetLoader.hpp"
#include "feyerverx/logger.hpp"
#include "feyerverx/renderer.hpp"
#include "feyerverx/texture.hpp"

namespace feyerverx {

static const uint16_t quadTriangles[] = {0, 2, 1, 0, 3, 2};

static void fillQuadFromRect(Vertex *o_out, const Rectangle &_rect) {
    float xMin = _rect.x;
    float xMax = _rect.x + _rect.width;

    float yMin = _rect.y;
    float yMax = _rect.y + _rect.height;

    o_out[0] = {xMin, yMin, 0.0f, 0.0f, 0.0f};
    o_out[1] = {xMax, yMin, 0.0f, 1.0f, 0.0f};
    o_out[2] = {xMax, yMax, 0.0f, 1.0f, 1.0f};
    o_out[3] = {xMin, yMax, 0.0f, 0.0f, 1.0f};
}

std::unique_ptr<Renderer> Renderer::create() {
    bgfx::VertexLayout layout;
    Vertex::InitLayout(layout);

    constexpr bgfx::VertexBufferHandle vertexBuffer = BGFX_INVALID_HANDLE;
    const bgfx::IndexBufferHandle trianglesBuffer =
        bgfx::createIndexBuffer(bgfx::makeRef(quadTriangles, sizeof(quadTriangles)));

    const bgfx::EmbeddedShader embeddedVertexShader = BGFX_EMBEDDED_SHADER(quad_vs);
    const bgfx::EmbeddedShader embeddedFragmentShader = BGFX_EMBEDDED_SHADER(quad_fs);

    const bgfx::ShaderHandle vertexShader =
        bgfx::createEmbeddedShader(&embeddedVertexShader, bgfx::getRendererType(), "quad_vs");
    const bgfx::ShaderHandle fragmentShader =
        bgfx::createEmbeddedShader(&embeddedFragmentShader, bgfx::getRendererType(), "quad_fs");
    const bgfx::ProgramHandle program = bgfx::createProgram(vertexShader, fragmentShader, true);

    const bgfx::UniformHandle textureSamplerUniform =
        bgfx::createUniform("s_texColor", bgfx::UniformType::Sampler);

    return std::unique_ptr<Renderer>(
        new Renderer{layout, vertexBuffer, trianglesBuffer, program, textureSamplerUniform});
}

Renderer::Renderer(Renderer &&other) noexcept {
    m_layout = other.m_layout;
    m_vertexBuffer = other.m_vertexBuffer;
    m_trianglesBuffer = other.m_trianglesBuffer;
    m_program = other.m_program;
    m_textureSamplerUniform = other.m_textureSamplerUniform;

    other.m_vertexBuffer = {bgfx::kInvalidHandle};
    other.m_trianglesBuffer = {bgfx::kInvalidHandle};
    other.m_program = {bgfx::kInvalidHandle};
    other.m_textureSamplerUniform = {bgfx::kInvalidHandle};
}

Renderer::Renderer(const bgfx::VertexLayout &layout, const bgfx::VertexBufferHandle &vertexBuffer,
                   const bgfx::IndexBufferHandle &trianglesBuffer,
                   const bgfx::ProgramHandle &program,
                   const bgfx::UniformHandle &textureSamplerUniform)
    : m_layout(layout), m_vertexBuffer(vertexBuffer), m_trianglesBuffer(trianglesBuffer),
      m_program(program), m_textureSamplerUniform(textureSamplerUniform) {
    Logger::instance().log(LogLevel::Info, "Renderer constructed!");
}

Renderer::~Renderer() {
    destroyHandle(m_textureSamplerUniform);
    destroyHandle(m_trianglesBuffer);
    destroyHandle(m_vertexBuffer);
    destroyHandle(m_program);
    Logger::instance().log(LogLevel::Info, "Destroyed renderer!");
}

void Renderer::renderTexture(const Texture &texture, const Rectangle &rectangle, uint16_t viewID) {
    Vertex quadVerticies[4];
    fillQuadFromRect(quadVerticies, rectangle);

    if (bgfx::isValid(m_vertexBuffer)) {
        bgfx::destroy(m_vertexBuffer);
    }

    // TODO: Do not create this each frame
    m_vertexBuffer =
        bgfx::createVertexBuffer(bgfx::copy(quadVerticies, sizeof(quadVerticies)), m_layout);

    bgfx::setVertexBuffer(0, m_vertexBuffer);
    bgfx::setIndexBuffer(m_trianglesBuffer);

    bgfx::setTexture(0, m_textureSamplerUniform, texture.handle());
    bgfx::setState(BGFX_STATE_DEFAULT | BGFX_STATE_BLEND_ALPHA);
    bgfx::submit(viewID, m_program);
}
}