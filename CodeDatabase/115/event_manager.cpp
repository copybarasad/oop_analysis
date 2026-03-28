#include "event_manager.h"
#include <algorithm>

std::vector<IObserver*> EventManager::observers;

void EventManager::addObserver(IObserver* observer) {
    observers.push_back(observer);
}

void EventManager::removeObserver(IObserver* observer) {
    auto it = std::remove(observers.begin(), observers.end(), observer);
    if (it != observers.end()) {
        observers.erase(it, observers.end());
    }
}

void EventManager::trigger(EventType type, const std::string& msg, int val1, int val2) {
    GameEvent event{type, msg, val1, val2};
    for (auto* obs : observers) {
        obs->onNotify(event);
    }
}