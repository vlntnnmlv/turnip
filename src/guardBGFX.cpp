// Copyright 2025 Valentin Namleev

#include <bgfx/bgfx.h>
#include <bgfx/platform.h>

#include "feyerverx/error.hpp"
#include "feyerverx/guardBGFX.hpp"
#include "feyerverx/logger.hpp"

namespace feyerverx {
GuardBGFX::GuardBGFX(void *windowHandle, int width, int height) {
    bgfx::renderFrame();

    bgfx::Init init;
    init.type = bgfx::RendererType::Count;

    init.platformData.nwh = windowHandle;
    init.resolution.width = width;
    init.resolution.height = height;
    init.resolution.reset = BGFX_RESET_VSYNC;

    init.platformData.ndt = nullptr;
    init.platformData.context = nullptr;
    init.platformData.backBuffer = nullptr;
    init.platformData.backBufferDS = nullptr;

    // On Apple's macOS, you must set the NSHighResolutionCapable Info.plist property to YES,
    // otherwise you will not receive a High-DPI OpenGL canvas.

    if (!bgfx::init(init)) {
        throw FeyError(FeyErrorType::BGFXInitializationError, "BGFX failed to initialize");
    }

    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff);
    bgfx::frame();

    Logger::instance().log(LogLevel::Info, "Inited BGFX!");
}

GuardBGFX::~GuardBGFX() { bgfx::shutdown(); }
}