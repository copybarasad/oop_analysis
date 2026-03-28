#include "../include/GameEvent.h"
#include <algorithm>

GameEventDispatcher& GameEventDispatcher::getInstance() {
    static GameEventDispatcher instance;
    return instance;
}

void GameEventDispatcher::addListener(IGameEventListener* listener) {
    if (listener) {
        auto it = std::find(listeners.begin(), listeners.end(), listener);
        if (it == listeners.end()) {
            listeners.push_back(listener);
        }
    }
}

void GameEventDispatcher::removeListener(IGameEventListener* listener) {
    auto it = std::find(listeners.begin(), listeners.end(), listener);
    if (it != listeners.end()) {
        listeners.erase(it);
    }
}

void GameEventDispatcher::dispatch(const GameEvent& event) {
    for (IGameEventListener* listener : listeners) {
        if (listener) {
            listener->onEvent(event);
        }
    }
}

void GameEventDispatcher::clear() {
    listeners.clear();
}
