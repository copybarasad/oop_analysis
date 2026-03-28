#ifndef COMMANDS_COMMAND_TYPE_H
#define COMMANDS_COMMAND_TYPE_H

enum class CommandType {
    MOVE_UP,
    MOVE_DOWN,
    MOVE_LEFT,
    MOVE_RIGHT,
    SWITCH_COMBAT,
    RANGED_ATTACK,
    CAST_SPELL,
    BUY_SPELL,
    SAVE_GAME,
    QUIT_GAME
};

#endif