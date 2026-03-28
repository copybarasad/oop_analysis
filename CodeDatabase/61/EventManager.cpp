#include "EventManager.h"
#include <algorithm>

EventManager& EventManager::getInstance() {
    static EventManager instance;
    return instance;
}

void EventManager::subscribe(IEventSubscriber* subscriber) {
    if (subscriber) {
        subscribers.push_back(subscriber);
    }
}

void EventManager::unsubscribe(IEventSubscriber* subscriber) {
    subscribers.erase(
        std::remove(subscribers.begin(), subscribers.end(), subscriber),
        subscribers.end()
    );
}

void EventManager::publish(const std::shared_ptr<GameEvent>& event) {
    if (!event) return;
    
    for (auto* subscriber : subscribers) {
        if (subscriber) {
            subscriber->onEvent(event);
        }
    }
}

void EventManager::clear() {
    subscribers.clear();
}

