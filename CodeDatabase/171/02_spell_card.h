#ifndef SPELL_CARD_H
#define SPELL_CARD_H

#include "01_position.h"
#include <string>

class SpellCard {
public:
    virtual ~SpellCard() = default;

    virtual bool cast(const Position& casterPos, const Position& targetPos) = 0;

    virtual std::string getName() const = 0;

    virtual std::string getDescription() const = 0;

    virtual int getRange() const = 0;

    virtual int getCost() const = 0;

    virtual bool canUse() const = 0;
};

#endif