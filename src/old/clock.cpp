// Copyright 2025 Valentin Namleev

#include "feyerverx/clock.hpp"

namespace feyerverx {
void Clock::update() {
    auto currentFrameTime = std::chrono::steady_clock::now();
    m_deltaTime = std::chrono::duration(currentFrameTime - m_previousFrameTime).count();
    m_elapsedTime = std::chrono::duration(currentFrameTime - m_startTime).count();
    m_previousFrameTime = currentFrameTime;
}

float Clock::elapsedTimeSeconds() { return m_elapsedTime / 1e9; }
float Clock::deltaTimeSeconds() { return m_deltaTime / 1e9; }
}