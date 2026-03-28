#ifndef SPELL_CARD_H
#define SPELL_CARD_H

#include "position.h"
#include <string>

class SpellCard {
public:
    virtual ~SpellCard() = default;
    
    virtual std::string getName() const = 0;
    virtual std::string getDescription() const = 0;
    virtual int getManaCost() const = 0;
    virtual int getRange() const = 0;
    virtual bool requiresTarget() const = 0;
    virtual void cast() = 0;
    virtual bool canCast(const Position& casterPosition, const Position& targetPosition) const = 0;
};

#endif