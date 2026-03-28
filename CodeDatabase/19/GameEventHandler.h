#pragma once

#pragma once
#include <vector>
#include "IEventListener.h"

class GameEventHandler {
public:
    void subscribe(IEventListener *listener) {
        listeners_.push_back(listener);
    }

    void unsubscribe(IEventListener *listener) {
        std::erase(listeners_, listener);
    }

    void notify(const GameEvent &event) const {
        for (IEventListener *listener: listeners_) {
            listener->handle_event(event);
        }
    }

private:
    std::vector<IEventListener *> listeners_;
};
