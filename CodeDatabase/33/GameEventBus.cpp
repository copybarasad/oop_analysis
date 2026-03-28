#include "GameEventBus.h"
#include <algorithm>

void GameEventBus::Subscribe(Listener* listener) {
    if (!listener) return;
    if (std::find(listeners_.begin(), listeners_.end(), listener) == listeners_.end()) {
        listeners_.push_back(listener);
    }
}

void GameEventBus::Unsubscribe(Listener* listener) {
    if (!listener) return;
    listeners_.erase(std::remove(listeners_.begin(), listeners_.end(), listener), listeners_.end());
}

void GameEventBus::Publish(const GameEvent& event) const {
    for (auto* listener : listeners_) {
        if (listener) {
            listener->OnEvent(event);
        }
    }
}
