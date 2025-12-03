// Copyright 2025 Valentin Namleev

#include "feyerverx/renderPass.hpp"

namespace feyerverx {
RenderPass::RenderPass(const RenderPassType renderPassType, const uint16_t viewID,
                       const uint8_t priority, const bool doClearDepth, const bool doClearColor,
                       const Color clearColor, const bgfx::FrameBufferHandle frameBuffer)
    : type(renderPassType), viewID(viewID), priority(priority), doClearDepth(doClearDepth),
      doClearColor(doClearColor), clearColor(clearColor), framebuffer(frameBuffer) {}
}