// Copyright 2025 Valentin Namleev

#include <bx/math.h>

#include "renderer.hpp"

namespace turnip {

static Vertex s_SproutQuad[] = {
    {-1.0f, 1.0f, 0.0f, 0.0f, 0.0f},
    {1.0f, 1.0f, 0.0f, 1.0f, 0.0f},
    {-1.0f, -1.0f, 0.0f, 0.0f, 1.0f},
    {1.0f, -1.0f, 0.0f, 1.0f, 1.0f},
};

static const uint16_t s_SoilIndices[] = {0, 1, 2, 2, 1, 3};

void Renderer::RenderTexture(bgfx::TextureHandle _textureHandle, bgfx::ProgramHandle _program) {
    static bgfx::VertexLayout sproutLayout;
    static bgfx::VertexBufferHandle sproutVB = BGFX_INVALID_HANDLE;
    static bgfx::IndexBufferHandle soilIB = BGFX_INVALID_HANDLE;
    static bgfx::UniformHandle texSampler = BGFX_INVALID_HANDLE;

    if (!bgfx::isValid(sproutVB)) {
        Vertex::InitLayout(sproutLayout);
        sproutVB = bgfx::createVertexBuffer(bgfx::makeRef(s_SproutQuad, sizeof(s_SproutQuad)),
                                            sproutLayout);
        soilIB = bgfx::createIndexBuffer(bgfx::makeRef(s_SoilIndices, sizeof(s_SoilIndices)));
        texSampler = bgfx::createUniform("s_Texture", bgfx::UniformType::Sampler);
    }

    // Ortho projection
    float ortho[16];
    bx::mtxOrtho(ortho, 0.0f, 1280.0f, 720.0f, 0.0f, 0.0f, 1000.0f, 0.0f, false);
    bgfx::setViewTransform(0, nullptr, ortho);
    bgfx::setViewRect(0, 0, 0, 1280, 720);

    float model[16];
    bx::mtxIdentity(model);
    bgfx::setTransform(model);

    bgfx::setVertexBuffer(0, sproutVB);
    bgfx::setIndexBuffer(soilIB);

    bgfx::setTexture(0, texSampler, _textureHandle);
    bgfx::setState(BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A);

    bgfx::submit(0, _program);
}
}