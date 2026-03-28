#pragma once
#include <string>
#include "Coord.h"

enum class GameEventType {
    PlayerMoved,
    DamageDealt,
    SpellAcquired,
    SpellCast,
    Spawned,
    ModeChanged,
    Saved,
    Loaded,
    TrapTriggered
};

struct GameEvent {
    GameEventType type;
    std::string source;
    std::string target;
    Coord position;
    int amount = 0;
    std::string details;
};
