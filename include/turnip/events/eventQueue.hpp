// Copyright 2025 Valentin Namleev

#pragma once

#include "./turnip/events/inputEvent.hpp"

#include <optional>
#include <vector>

namespace turnip::events {
class EventQueue {
public:
    EventQueue() = default;
    ~EventQueue() = default;

    void Push(const InputEvent &_Event);
    const std::vector<InputEvent> &Poll() const;
    std::optional<InputEvent> Pop();
    void Clear();

private:
    std::vector<InputEvent> m_Queue;
};
} // namespace turnip::events
