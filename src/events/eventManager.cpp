// Copyright 2025 Valentin Namleev

#include "feyerverx/events/eventManager.hpp"

namespace feyerverx {
void EventManager::dispatch() {
    for (auto &event : m_eventQueue) {
        // it's okay to use dereferencing here, because event will nave have * operator overridden
        auto &type = typeid(*event);
        auto it = m_handlers.find(type);
        if (it == m_handlers.end())
            continue;
        for (auto &callback : it->second)
            callback(*event);
    }

    m_eventQueue.clear();
}
} // namespace feyerverx