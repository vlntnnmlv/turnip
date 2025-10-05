// Copyright 2025 Valentin Namleev

#pragma once

#include <chrono>

namespace feyerverx {
class Clock {
public:
    void update();

    float elapsedTimeSeconds();
    float deltaTimeSeconds();

private:
    std::chrono::steady_clock::time_point m_startTime = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point m_previousFrameTime = std::chrono::steady_clock::now();

    float m_elapsedTime = std::chrono::duration<float>().count();
    float m_deltaTime = std::chrono::duration<float>().count();
};
} // namespace feyerverx
