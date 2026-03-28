#pragma once
#include "field/position.h"
#include <string>

enum class EventType
{
    PlayerMoved,
    PlayerAttacked,
    PlayerReceivedSpell,
    EnemyAttacked,
    SpellUsed,
    GameSaved,
    GameLoaded
};

struct Event
{
    EventType type;
    std::string message;
    Position position;
    int damage = 0;
    std::string entityName;
};