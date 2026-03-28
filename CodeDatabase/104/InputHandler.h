#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include "SpellCastInfo.h"
#include <string>

class Player;
class GameField;

class InputHandler {
public:
    struct PlayerInput {
        enum class Type {
            MOVE,
            CAST_SPELL,
            QUIT,
            INVALID
        };

        Type type;
        int move_dx;
        int move_dy;
        size_t spell_index;
    };

    InputHandler() = default;

    PlayerInput parseInput(const std::string& input) const;
    SpellCastInfo getSpellTarget(const Player& player, const GameField& field) const;
};

#endif