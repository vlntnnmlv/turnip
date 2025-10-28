// Copyright 2025 Valentin Namleev

#pragma once

#include <functional>
#include <memory>

#include "feyerverx/renderPass.hpp"

namespace feyerverx {
class RenderPassManager {
public:
    using RenderCallback = std::function<void(uint16_t viewID)>;

    static std::unique_ptr<RenderPassManager> create(uint32_t width, uint32_t height);

    void addPass(RenderPassType renderPassType, uint8_t priority = 0,
                 bgfx::FrameBufferHandle frameBuffer = BGFX_INVALID_HANDLE);

    RenderPass *getPass(RenderPassType renderPassType);
    void executePass(RenderPassType renderPassType, const RenderCallback &renderCallback);

    bgfx::FrameBufferHandle createFrameBuffer(uint32_t width, uint32_t height,
                                              bool withDepth = true);
    void destroyFrameBuffer(bgfx::FrameBufferHandle frameBuffer);

    ~RenderPassManager();

private:
    std::vector<RenderPass> m_passes;
    uint16_t m_nextViewID = 0;
    uint32_t m_width;
    uint32_t m_height;

    RenderPassManager(uint32_t width, uint32_t height);
    void setupPass(const RenderPass &renderPass);
};
} // namespace feyerverx
