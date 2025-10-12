// Copyright October 2025 Valentin Namleev

#pragma once
#include "SDL3/SDL_keycode.h"

#include <functional>

namespace feyerverx {
template <typename Event> struct EventHandler {
    using Callback = std::function<void(Event)>;

    void addCallback(const Callback &callback) { callbacks.push_back(callback); }
    void addCallback(Callback &&callback) { callbacks.push_back(std::move(callback)); }

    void invoke(const Event &event) const {
        for (auto &callback : callbacks) {
            callback(event);
        }
    }

    std::vector<Callback> callbacks{};
};

class EventSink {
public:
    static EventHandler<SDL_Keycode> OnKeyDown;
};
}