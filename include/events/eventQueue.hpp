// Copyright 2025 Valentin Namleev

#pragma once

#include "./inputEvent.hpp"

#include <optional>
#include <vector>

namespace turnip::events {
class EventQueue {
public:
    EventQueue() = default;
    ~EventQueue() = default;

    void Push(const InputEvent &_Event) { m_Queue.push_back(_Event); }

    const std::vector<InputEvent> &Poll() const { return m_Queue; }

    std::optional<InputEvent> Pop() {
        if (m_Queue.empty())
            return std::nullopt;

        InputEvent event = std::move(m_Queue.front());
        m_Queue.erase(m_Queue.begin());
        return event;
    }

    void Clear() { m_Queue.clear(); }

private:
    std::vector<InputEvent> m_Queue;
};
} // namespace turnip::events
