#ifndef COMMANDTYPE_H
#define COMMANDTYPE_H

enum class CommandType {
    MOVE_UP,
    MOVE_DOWN, 
    MOVE_LEFT,
    MOVE_RIGHT,
    SWITCH_COMBAT_MODE,
    CAST_SPELL,
    BUY_SPELL,
    SAVE_GAME,
    LOAD_GAME,
    QUIT_GAME
};

#endif