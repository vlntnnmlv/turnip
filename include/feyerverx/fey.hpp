// Copyright 2025 Valentin Namleev

#pragma once

#include <memory>
#include <optional>
#include <string>

#include <SDL3/SDL.h>
#include <bgfx/platform.h>
#include <bx/math.h>

#include "feyerverx/camera.hpp"
#include "feyerverx/guardBGFX.hpp"
#include "feyerverx/guardSDL.hpp"
#include "feyerverx/scene.hpp"

namespace feyerverx {
class Fey {
public:
    Fey(const std::string &title, int _width, int _height);
    ~Fey();

    void run();

private:
    void initCamera();

private:
    std::string m_title;
    int m_width;
    int m_height;

    bool m_running;

    GuardSDL m_guardSDL;
    GuardBGFX m_guardBGFX;

    std::unique_ptr<ICamera> m_camera;

    Scene m_scene; // There should be an stack of scenes. Some scenes are acitve, some suspended.
                   // Each scene may have it's own camera.
};
} // namespace feyerverx