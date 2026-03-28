#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include "Position.h"

enum class GameAction {
    MOVE_UP,
    MOVE_DOWN,
    MOVE_LEFT,
    MOVE_RIGHT,
    SWITCH_COMBAT_MODE,
    RANGED_ATTACK,  
    QUIT_GAME,
    INVALID_ACTION,
    CAST_SPELL_1,
    CAST_SPELL_2,
    CAST_SPELL_3,
    SAVE_GAME, 
};

class InputHandler {
public:
    GameAction getNextAction() const;
    Position calculateMoveDirection(GameAction action) const;
};

#endif