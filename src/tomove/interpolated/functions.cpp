#include "./turnip/interpolated/functions.hpp"
#include <cmath>
#include <cstdint>

namespace turnip {
float SimplePow(float x, uint32_t p) {
    float res = 1.0f;
    for (uint32_t i(p); i--;) {
        res *= x;
    }
    return res;
}

float Linear(float t) { return t; }

float EaseInOutExponential(float t) {
    if (t < 0.5f) {
        return std::pow(2.0f, 20.0f * t - 10.0f) * 0.5f;
    }
    return (2.0f - std::pow(2.0f, -20.0f * t + 10.0f)) * 0.5f;
}

float EaseOutBack(float t) {
    constexpr float c1 = 1.70158f;
    constexpr float c3 = c1 + 1.0f;
    return 1.0f + c3 * SimplePow(t - 1.0f, 3) + c1 * SimplePow(t - 1.0f, 2);
}

float EaseInOutQuint(float t) {
    if (t < 0.5f) {
        return 16.0f * SimplePow(t, 5);
    }
    return 1.0f - SimplePow(-2.0f * t + 2, 5) * 0.5f;
}

float EaseInBack(float t) {
    float constexpr c1 = 1.70158f;
    float constexpr c3 = c1 + 1.0f;
    return c3 * t * t * t - c1 * t * t;
}

float EaseOutElastic(float t) {
    float constexpr two_pi = 2.0f * 3.14159265359f;
    float constexpr c4 = two_pi / 3.0f;
    if (t == 0.0f) {
        return 0.0f;
    }
    if (t == 1.0f) {
        return 1.0f;
    }
    return std::pow(2.0f, -10.0f * t) * std::sin((t * 10.0f - 0.75f) * c4) + 1.0f;
}

float GetRatio(float t, TransitionFunction transition) {
    switch (transition) {
    default:
        return t;
    case TransitionFunction::None:
        return 1.0f;
    case TransitionFunction::Linear:
        return t;
    case TransitionFunction::EaseInOutExponential:
        return EaseInOutExponential(t);
    case TransitionFunction::EaseOutBack:
        return EaseOutBack(t);
    case TransitionFunction::EaseOutElastic:
        return EaseOutElastic(t);
    }
}
}