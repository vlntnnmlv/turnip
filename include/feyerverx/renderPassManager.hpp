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

    void addPass();
    void getPass();
    void executePass();
    void createFrameBuffer();
    void destroyFrameBuffer();

    ~RenderPassManager();

private:
    std::vector<RenderPass> m_passes;
    uint16_t m_nextViewID = 0;
    uint32_t m_width;
    uint32_t m_height;

    RenderPassManager(uint32_t width, uint32_t height);
    void setupPass();
};
} // namespace feyerverx
