#include "Notifier.h"
#include <algorithm>
#include <iostream>

void Notifier::subscribe(std::shared_ptr<IObserver> observer) {
    auto it = std::find(observers.begin(), observers.end(), observer);
    if (it == observers.end()) {
        observers.push_back(observer);
    }
}

void Notifier::unsubscribe(std::shared_ptr<IObserver> observer) {
    auto it = std::find(observers.begin(), observers.end(), observer);
    if (it != observers.end()) {
        observers.erase(it);
    }
}

void Notifier::notify(const Event& event) {
    for (std::shared_ptr<IObserver> observer : observers) {
        observer->seeEvent(event);
    }
}

Notifier& Notifier::getInstance() {
    static Notifier instance;
    return instance;
}



