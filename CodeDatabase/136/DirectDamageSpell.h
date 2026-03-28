#ifndef DIRECT_DAMAGE_SPELL_H
#define DIRECT_DAMAGE_SPELL_H

#include "Spell.h"

class DirectDamageSpell : public ISpell {
private:
    std::string name;
    std::string description;
    int damage;
    int manaCost;
    int range;

public:
    DirectDamageSpell(std::string name, int damage, int manaCost = 2, int range = 3);
    
    void cast(Position target, GameField& field) override;
    std::string getName() const override { return name; }
    std::string getDescription() const override { return description; }
    int getManaCost() const override { return manaCost; }
    int getRange() const override { return range; }
    SpellType getType() const override { return SpellType::DIRECT_DAMAGE; }
};

#endif