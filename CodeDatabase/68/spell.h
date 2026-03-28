#ifndef SPELL_H
#define SPELL_H

#include "Position.h"
#include <string>

class GameField; // Forward declaration

class Spell {
public:
    virtual ~Spell() = default;
    virtual void cast(GameField* field, const Position& targetPosition) = 0;
    virtual std::string getName() const = 0;
    virtual int getManaCost() const = 0;
    virtual int getDamage() const = 0;
    virtual int getRange() const = 0;
    virtual void upgradeDamage(int additionalDamage) = 0;
    virtual void upgradeRange(int additionalRange) = 0;
    virtual void reduceManaCost(int reduction) = 0;
};

#endif