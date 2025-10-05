// Copyright 2025 Valentin Namleev

#include "feyerverx/events/eventQueue.hpp"

namespace feyerverx::events {
void EventQueue::Push(const InputEvent &_Event) { m_Queue.push_back(_Event); }

const std::vector<InputEvent> &EventQueue::Poll() const { return m_Queue; }

std::optional<InputEvent> EventQueue::Pop() {
    if (m_Queue.empty())
        return std::nullopt;

    InputEvent event = std::move(m_Queue.front());
    m_Queue.erase(m_Queue.begin());
    return event;
}

void EventQueue::Clear() { m_Queue.clear(); }
} // namespace feyerverx::events
