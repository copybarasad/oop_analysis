#ifndef SPELL_H
#define SPELL_H

#include "Position.h"
#include <string>

enum class SpellType {
    FIREBALL,
    EXPLOSION, 
    TRAP,
    INVISIBILITY,
    SUMMON,  
    ENHANCE
};

class Spell {
public:
    virtual ~Spell() = default;
    
    virtual SpellType getType() const = 0;
    virtual std::string getName() const = 0;
    virtual int getDamage() const = 0;
    virtual int getRange() const = 0;
    virtual int getArea() const = 0;
    virtual int getManaCost() const = 0;
    
    virtual void cast(const Position& casterPos, const Position& targetPos) = 0;
    virtual Spell* clone() const = 0;
    
    virtual void enhanceDamage(int amount) = 0;
    virtual void enhanceRange(int amount) = 0;
    virtual void enhanceArea(int amount) = 0;
    virtual void enhanceDuration(int amount) = 0;
    virtual void enhanceSummonCount(int amount) = 0;
};

#endif