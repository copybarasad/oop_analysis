#include "event_system.h"
#include <algorithm>

void EventSystem::subscribe(EventListener* listener) {
    if (listener) {
        listeners_.push_back(listener);
    }
}

void EventSystem::unsubscribe(EventListener* listener) {
    listeners_.erase(
        std::remove(listeners_.begin(), listeners_.end(), listener),
        listeners_.end()
    );
}

void EventSystem::notify(const GameEvent& event) const {
    for (EventListener* listener : listeners_) {
        listener->onEvent(event);
    }
}