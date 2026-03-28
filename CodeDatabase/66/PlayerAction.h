#ifndef PLAYERACTION_H
#define PLAYERACTION_H

#include "Direction.h"
#include "Position.h"

enum class ActionType {
    MOVE,
    ATTACK,
    SWITCH_MODE,
    SKIP_TURN,
    CAST_SPELL,
    BUY_SPELL,
    SAVE_GAME,
    LOAD_GAME,
    RESTART,
    QUIT,
    INVALID
};

struct PlayerAction {
    ActionType type;
    Direction direction;
    int spellIndex;
    Position targetPosition;
    
    PlayerAction(ActionType t = ActionType::INVALID, Direction d = Direction::NONE, 
                int spell = -1, Position target = Position(0, 0))
        : type(t), direction(d), spellIndex(spell), targetPosition(target) {}
};

#endif
