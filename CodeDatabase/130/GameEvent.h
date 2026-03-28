#ifndef GAME_EVENT_H
#define GAME_EVENT_H

#include <string>

enum class GameEventType {
    DAMAGE,
    MOVE,
    SPELL_CAST,
    SPELL_DRAW,
    SAVE,
    LOAD
};

struct GameEvent {
    GameEventType type;
    std::string message;
};

#endif


