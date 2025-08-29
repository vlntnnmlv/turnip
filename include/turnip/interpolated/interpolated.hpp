#pragma once

#include "./turnip/interpolated/functions.hpp"
#include "./turnip/rectangleUtils.hpp"

#include <chrono>

namespace turnip {
/** An object that implements automatic interpolation on value changes.
 *  It can be used as a drop in replacement thanks to cast and assign operators.
 */
template <typename T> struct Interpolated {
    /// The value at the start of the transition
    T m_Start{};
    /// The target value
    T m_End{};
    /// The transition start timestamp
    float m_StartTime{};
    /// The animation's speed
    float m_Speed{30.0f};
    /// The transition function to use
    TransitionFunction m_Transition{TransitionFunction::EaseInBack};

    /// Initializes the value with @p initial_value
    explicit Interpolated(T const &_InitialValue = {}) : m_Start{_InitialValue}, m_End{m_Start} {}

    /// Returns stop watch time (should be better compared to UTC timestamps for float precision)
    [[nodiscard]]
    static float GetCurrentTime() {
        return (float)GetTime();
        // Retrieve current time
        // auto const now = std::chrono::steady_clock::now();
        // auto const duration = now.time_since_epoch();
        // // Convert it to a decimal number of seconds
        // auto const seconds = std::chrono::duration_cast<std::chrono::duration<float>>(duration);
        // return seconds.count();
    }

    /// Returns the number of seconds since the last value change
    [[nodiscard]]
    float GetElapsedSeconds() const {
        return GetCurrentTime() - m_StartTime;
    }

    /// Sets a new target value and resets transition
    void SetValue(T const &_NewValue) {
        m_Start = GetValue();
        m_End = _NewValue;
        m_StartTime = GetCurrentTime();
    }

    /// Returns the current value
    [[nodiscard]]
    T GetValue() const {
        // Current transition time
        float const elapsed = GetElapsedSeconds();
        float const t = elapsed * m_Speed;
        // Check if the transition is over
        if (t >= 1.0f) {
            // If the transition is done, directly return the target value
            return m_End;
        }
        // Else compute interpolated value and return it
        T const delta{m_End - m_Start};
        return m_Start + delta * GetRatio(t, m_Transition);
    }

    [[nodiscard]] const T *operator->() const { return &GetValue(); }
    [[nodiscard]] T *operator->() {
        // Optional: allow mutable access to .m_End directly or assert it's intentional
        return &m_End;
    }
    [[nodiscard]] const T &operator*() const { return GetValue(); }

    /// Computes the speed given a duration
    void SetDuration(float duration) { m_Speed = 1.0f / duration; }

    /// Cast operator to use this object directly as if it was of type T
    [[nodiscard]] operator T() const { return GetValue(); }

    /// Assign operator to ease transitions
    void operator=(T const &_NewValue) { SetValue(_NewValue); }
};
}