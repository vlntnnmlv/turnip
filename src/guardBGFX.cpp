// Copyright 2025 Valentin Namleev

#include <bgfx/bgfx.h>
#include <bgfx/platform.h>

#include "feyerverx/guardBGFX.hpp"
#include "feyerverx/logger.hpp"

namespace feyerverx {
std::expected<std::unique_ptr<GuardBGFX>, Error>
GuardBGFX::create(void *windowHandle, const float width, const float height) {

    bgfx::renderFrame();

    bgfx::Init init;
    init.type = bgfx::RendererType::Count;

    init.platformData.nwh = windowHandle;
    init.resolution.width = static_cast<uint32_t>(width);
    init.resolution.height = static_cast<uint32_t>(height);
    init.resolution.reset = BGFX_RESET_VSYNC;

    init.platformData.ndt = nullptr;
    init.platformData.context = nullptr;
    init.platformData.backBuffer = nullptr;
    init.platformData.backBufferDS = nullptr;

    // On Apple's macOS, you must set the NSHighResolutionCapable Info.plist property to YES,
    // otherwise you will not receive a High-DPI OpenGL canvas.
    if (!bgfx::init(init)) {
        return std::unexpected{
            Error{ErrorType::BGFXInitializationError, "BGFX failed to initialize"}};
    }

    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff);
    bgfx::frame();

    Logger::instance().log(LogLevel::Info, "Initialized BGFX!");

    return std::unique_ptr<GuardBGFX>(new GuardBGFX{});
}

GuardBGFX::GuardBGFX() { Logger::instance().log(LogLevel::Info, "BGFXGuard constructed!"); }

GuardBGFX::~GuardBGFX() {
    bgfx::shutdown();
    Logger::instance().log(LogLevel::Info, "BGFXGuard destroyed!");
}
}