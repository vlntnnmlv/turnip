// Copyright 2025 Valentin Namleev

#include "feyerverx/renderPassManager.hpp"
#include "feyerverx/logger.hpp"

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
}