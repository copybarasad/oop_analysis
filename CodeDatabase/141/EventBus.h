#pragma once

#include "Event.h"
#include <functional>
#include <vector>

class EventBus {
public:
    using Listener = std::function<void(const GameEvent &)>;

    void subscribe(const Listener &listener) {
        listeners.push_back(listener);
    }

    void publish(const GameEvent &event) const {
        for (const auto &listener: listeners) {
            listener(event);
        }
    }

private:
    std::vector<Listener> listeners;
};
