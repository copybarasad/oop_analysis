#ifndef SPELL_H
#define SPELL_H

#include "SpellTarget.h"
#include "SpellType.h"
#include "EnhancementBuffer.h"
#include <string>

class GameWorld;

class Spell {
public:
    virtual ~Spell() = default;
    
    virtual bool canCast(const Position& playerPos, const SpellTarget& target, const GameWorld& world) const = 0;
    virtual void cast(const Position& playerPos, const SpellTarget& target, GameWorld& world, EnhancementBuffer& buffer) = 0;
    virtual SpellType getType() const = 0;
    virtual std::string getName() const = 0;
    virtual std::string getDescription(const EnhancementBuffer& buffer) const = 0;
};

#endif
