#ifndef DIRECT_DAMAGE_SPELL_H
#define DIRECT_DAMAGE_SPELL_H

#include "Spell.h"

class DirectDamageSpell : public Spell {
private:
    std::string name;
    std::string description;
    int damage;
    int range;

public:
    DirectDamageSpell();
    
    std::string getName() const override { return name; }
    std::string getDescription() const override { return description; }
    bool canCast(const Position& casterPos, const Position& targetPos, const GameField& field) const override;
    void cast(const Position& casterPos, const Position& targetPos, GameField& field) override;
    int getRange() const override { return range; }
    int getDamage() const { return damage; } 
};

#endif