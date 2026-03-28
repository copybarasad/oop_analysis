#pragma once
#include "GameEvent.h"

class IGameObserver {
public:
    virtual ~IGameObserver() = default;
    virtual void OnGameEvent(const GameEvent& event) = 0;
};
