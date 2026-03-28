#pragma once
#include "GameEvent.h"

class IEventListener {
public:
    virtual ~IEventListener() = default;

    virtual void handle_event(const GameEvent &event) = 0;
};
