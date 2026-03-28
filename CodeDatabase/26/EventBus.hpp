#pragma once

#include <vector>

#include "GameEvent.hpp"
#include "GameEventListener.hpp"

class EventBus {
public:
	void subscribe(GameEventListener& listener);
	void unsubscribe(GameEventListener& listener);
	void publish(const GameEvent& event);

private:
	std::vector<GameEventListener*> listeners_;
};
