#ifndef EVENT_LISTENER_H
#define EVENT_LISTENER_H

#include "game_event.h"

class IEventListener
{
public:
    virtual ~IEventListener() = default;
    virtual void on_event(const GameEvent& event) = 0;
};

#endif