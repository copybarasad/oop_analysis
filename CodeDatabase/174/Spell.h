#ifndef SPELL_H
#define SPELL_H

#include "Position.h"
#include <string>

class GameField;
class Player;
class Game;

class Spell {
public:
    virtual ~Spell() = default;

    virtual bool cast(GameField& field, const Position& targetPos, Player& player, Game& game) = 0;
    virtual std::string getName() const = 0;
    virtual bool isBuffSpell() const { return false; }
};

#endif