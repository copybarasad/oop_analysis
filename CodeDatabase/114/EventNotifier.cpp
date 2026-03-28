#include "EventNotifier.h"
#include <algorithm>

void EventNotifier::subscribe(IObserver* observer) {
    if (observer == nullptr) {
        return;
    }
    auto it = std::find(observers.begin(), observers.end(), observer);
    if (it == observers.end()) {
        observers.push_back(observer);
    }
}

void EventNotifier::unsubscribe(IObserver* observer) {
    if (observer == nullptr) {
        return;
    }
    auto it = std::find(observers.begin(), observers.end(), observer);
    if (it != observers.end()) {
        observers.erase(it);
    }
}

void EventNotifier::notify(const GameEvent& event) {
    for (IObserver* observer : observers) {
        if (observer != nullptr) {
            observer->onEvent(event);
        }
    }
}

EventNotifier& EventNotifier::getInstance() {
    static EventNotifier instance;
    return instance;
}

