#ifndef COMMAND_H
#define COMMAND_H

#include <string>

enum class CommandType {
    MOVE_UP,
    MOVE_DOWN,
    MOVE_LEFT,
    MOVE_RIGHT,
    SWITCH_COMBAT_MODE,
    ATTACK,
    CAST_SPELL,
    PURCHASE_SPELL,
    SAVE_GAME,
    LOAD_GAME,
    HELP,
    QUIT,
    UNKNOWN
};

#endif

