// Copyright 2025 Valentin Namleev

#pragma once

#include "bgfx/bgfx.h"
#include "color.hpp"

#include <cstdint>

namespace feyerverx {

enum class RenderPassType {
    DepthPrePass,
    Geometry,
    Lighting,
    PostProcess,
    UI,

    Count
};

struct RenderPass {
    RenderPassType type;
    uint16_t viewID;
    uint8_t priority;
    bool doClearDepth;
    bool doClearColor;
    Color clearColor;

    bgfx::FrameBufferHandle framebuffer;

    RenderPass(RenderPassType renderPassType, uint16_t viewID, uint8_t priority = 0,
               bool clearDepth = true, bool doClearColor = true, Color clearColor = {0, 0, 0, 1},
               bgfx::FrameBufferHandle frameBuffer = BGFX_INVALID_HANDLE);
};
} // namespace feyerverx