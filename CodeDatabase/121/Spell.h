#ifndef SPELL_H
#define SPELL_H

#include "Position.h"
#include <string>

class GameField;

class Spell {
public:
    virtual ~Spell() = default;
    virtual std::string getName() const = 0;
    virtual std::string getDescription() const = 0;
    virtual bool canCast(const Position& casterPos, const Position& targetPos, const GameField& field) const = 0;
    virtual void cast(const Position& casterPos, const Position& targetPos, GameField& field) = 0;
    virtual int getRange() const = 0;
};

#endif