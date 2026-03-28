//
// Created by Mac on 02.12.2025.
//

#ifndef GAME_TERM_COMMANDTYPE_H
#define GAME_TERM_COMMANDTYPE_H

enum class CommandType {
    MOVE_UP,
    MOVE_DOWN,
    MOVE_LEFT,
    MOVE_RIGHT,
    SWITCH_FIGHT_MODE,
    SHOOT,
    BUY_SPELL,
    USE_SPELL,
    QUIT_GAME,
    SAVE_GAME,
    UNKNOWN
};

#endif //GAME_TERM_COMMANDTYPE_H