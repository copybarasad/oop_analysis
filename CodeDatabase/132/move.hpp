#pragma once
#include <memory>
#include "position.hpp"
#include "weapon.hpp"

enum MoveType {
    ATTACK,
    MOVE,
    CHANGE_WEAPON,
    SPELL
};

class Move
{
public:
    Position from          = NO_POSITION;
    Position to            = NO_POSITION;
    int spell_index        = -1;
    MoveType type          = MoveType::MOVE;
    CombatMode combat_mode = CombatMode::NONE;

    Move() = default;
    Move(Position from, Position to, MoveType type);
    Move(Position from, Position to, MoveType type, int spell_index);
    // Move(Position pos);
    // Move(Position from, Position to);
    // Move(Position from, Position to, CombatMode combat_mode);
    // Move(Position from, Position to, int spell_index);
};