#include "event_bus.h"
#include <algorithm>

EventBus* EventBus::instance = nullptr;

EventBus& EventBus::getInstance() {
    if (!instance) {
        instance = new EventBus();
    }
    return *instance;
}

void EventBus::subscribe(std::shared_ptr<EventListener> listener) {
    if (!listener) {
        return;
    }
    
    for (const auto& existing : listeners) {
        if (existing == listener) {
            return;
        }
    }
    listeners.push_back(listener);
}

void EventBus::unsubscribe(std::shared_ptr<EventListener> listener) {
    listeners.erase(
        std::remove(listeners.begin(), listeners.end(), listener),
        listeners.end()
    );
}

void EventBus::publish(const std::shared_ptr<GameEvent>& event) {
    if (!event) {
        return;
    }
    
    for (auto& listener : listeners) {
        if (listener) {
            listener->onEvent(event);
        }
    }
}

void EventBus::clearListeners() {
    listeners.clear();
}