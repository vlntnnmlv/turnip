// Copyright October 2025 Valentin Namleev

#pragma once

#include <functional>
#include <queue>
#include <typeindex>

#include "feyerverx/events/event.hpp"

#include <list>

namespace feyerverx {
class EventManager {
public:
    template <typename TEvent> using Handler = std::function<bool(const TEvent &)>;

    using Handlers = std::unordered_map<std::type_index, std::vector<Handler<Event>>>;
    using EventQueue = std::list<std::unique_ptr<Event>>;


   template <typename TEvent> void subscribe(Handler<TEvent> handler) {
        auto &handlers = m_handlers[typeid(TEvent)];
        handlers.push_back(
            [handler](const Event &e) -> bool { return handler(static_cast<const TEvent &>(e)); });
    }

    template <typename TEvent, typename... Args> void queueEvent(Args &&...args) {
        m_eventQueue.push_back(std::make_unique<TEvent>(std::forward<Args>(args)...));
    }

    template <typename TEvent> void queueEvent(const TEvent &event) {
        m_eventQueue.push_back(std::make_unique<TEvent>(event));
    }

    void dispatch();

private:
    Handlers m_handlers;
    EventQueue m_eventQueue;
};
}