#ifndef SPELL_H
#define SPELL_H

#include "Position.h"
#include <string>

class GameState;
class Player;

class Spell {
public:
    virtual ~Spell() = default; 

    virtual bool cast(const Position& target, GameState& gameState, const Player& caster) = 0;
    virtual std::string getName() const = 0;
    virtual int getManaCost() const = 0;
    virtual int getRange() const = 0;
    virtual int getDamage() const = 0;
    virtual bool requiresTarget() const = 0;

    bool canCast(const Position& casterPos, const Position& target, const GameState& gameState) const;
    int calculateDistance(const Position& casterPos, const Position& target) const;
};

#endif