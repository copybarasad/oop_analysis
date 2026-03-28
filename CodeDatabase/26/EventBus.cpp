#include "EventBus.hpp"

#include <algorithm>

void EventBus::subscribe(GameEventListener& listener) {
	if (std::find(listeners_.begin(), listeners_.end(), &listener) == listeners_.end()) {
		listeners_.push_back(&listener);
	}
}

void EventBus::unsubscribe(GameEventListener& listener) {
	auto it = std::remove(listeners_.begin(), listeners_.end(), &listener);
	listeners_.erase(it, listeners_.end());
}

void EventBus::publish(const GameEvent& event) {
	for (auto* listener : listeners_) {
		if (listener) {
			listener->onEvent(event);
		}
	}
}
