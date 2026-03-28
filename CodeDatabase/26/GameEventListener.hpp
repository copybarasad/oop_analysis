#pragma once

#include "GameEvent.hpp"

class GameEventListener {
public:
	virtual ~GameEventListener() = default;
	virtual void onEvent(const GameEvent& event) = 0;
};
