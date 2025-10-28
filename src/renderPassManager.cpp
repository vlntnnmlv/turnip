// Copyright 2025 Valentin Namleev

#include <algorithm>

#include "feyerverx/logger.hpp"
#include "feyerverx/renderPassManager.hpp"

namespace feyerverx {
std::unique_ptr<RenderPassManager> RenderPassManager::create(const uint32_t width,
                                                             const uint32_t height) {
    return std::unique_ptr<RenderPassManager>(new RenderPassManager{width, height});
}

RenderPassManager::RenderPassManager(const uint32_t width, const uint32_t height)
    : m_width(width), m_height(height) {
    Logger::instance().log(LogLevel::Info, "RenderPassManager created");
}

RenderPassManager::~RenderPassManager() {
    for (const auto &pass : m_passes) {
        if (bgfx::isValid(pass.framebuffer)) {
            bgfx::destroy(pass.framebuffer);
        }
    }
}

void RenderPassManager::addPass(RenderPassType renderPassType, uint8_t priority,
                                bgfx::FrameBufferHandle frameBuffer) {
    m_passes.emplace_back(renderPassType, m_nextViewID++, priority, true, true, Color{0, 0, 0, 255},
                          frameBuffer);

    std::ranges::sort(m_passes,
                      [](const auto &lhs, const auto &rhs) { return lhs.priority < rhs.priority; });
}

RenderPass *RenderPassManager::getPass(RenderPassType renderPassType) {
    const auto it = std::ranges::find_if(m_passes, [renderPassType](const auto &renderPass) {
        return renderPass.type == renderPassType;
    });

    return it != m_passes.end() ? &(*it) : nullptr;
}

void RenderPassManager::executePass(const RenderPassType renderPassType,
                                    const RenderCallback &renderCallback) {
    if (const auto *renderPass = getPass(renderPassType)) {
        setupPass(*renderPass);
        renderCallback(renderPass->viewID);
    }
}

void RenderPassManager::setupPass(const RenderPass &renderPass) {
    if (bgfx::isValid(renderPass.framebuffer)) {
        bgfx::setViewFrameBuffer(renderPass.viewID, renderPass.framebuffer);
    } else {
        bgfx::setViewFrameBuffer(renderPass.viewID, BGFX_INVALID_HANDLE);
    }

    if (renderPass.clearColor || renderPass.doClearDepth) {
        uint16_t clearFlags = 0;
        if (renderPass.doClearColor)
            clearFlags |= BGFX_CLEAR_COLOR;
        if (renderPass.doClearDepth)
            clearFlags |= BGFX_CLEAR_DEPTH;

        bgfx::setViewClear(renderPass.viewID, clearFlags, renderPass.clearColor);
    }

    bgfx::touch(renderPass.viewID);
}

bgfx::FrameBufferHandle RenderPassManager::createFrameBuffer(const uint32_t width,
                                                             const uint32_t height,
                                                             const bool withDepth) {
    const uint64_t textureFlags = BGFX_TEXTURE_RT | BGFX_SAMPLER_U_CLAMP | BGFX_SAMPLER_V_CLAMP;

    const bgfx::TextureFormat::Enum colorFormat = bgfx::TextureFormat::RGBA16F;

    const bgfx::TextureHandle colorTexture =
        bgfx::createTexture2D(width, height, false, 1, colorFormat, textureFlags);

    bgfx::TextureHandle depthTexture = BGFX_INVALID_HANDLE;
    if (withDepth) {
        depthTexture = bgfx::createTexture2D(width, height, false, 1, bgfx::TextureFormat::D24S8,
                                             textureFlags);
    }
    const bgfx::TextureHandle textures[] = {colorTexture, depthTexture};
    return bgfx::createFrameBuffer(withDepth ? 2 : 1, textures, true);
}

void RenderPassManager::destroyFrameBuffer(bgfx::FrameBufferHandle frameBuffer) {
    if (bgfx::isValid(frameBuffer)) {
        bgfx::destroy(frameBuffer);
    }
}

}