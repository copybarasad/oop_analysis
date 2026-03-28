#ifndef EVENT_LISTENER_H
#define EVENT_LISTENER_H

#include "game_event.h"
#include <memory>

class EventListener {
public:
    virtual ~EventListener() = default;
    virtual void onEvent(const std::shared_ptr<GameEvent>& event) = 0;
};

#endif