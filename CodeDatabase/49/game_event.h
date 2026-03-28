#ifndef GAME_EVENT_H
#define GAME_EVENT_H

#include <string>
#include "game_event_type.h"

struct GameEvent
{
    GameEventType type;
    std::string description;
    
    GameEvent(GameEventType t, const std::string& desc)
        : type(t), description(desc) {}
};

#endif