#ifndef TRAP_SPELL_H
#define TRAP_SPELL_H

#include "Spell.h"
#include "GameField.h"
#include <string>

class TrapSpell : public ISpell {
private:
    std::string name;
    std::string description;
    int damage;
    int manaCost;
    int range;

public:
    TrapSpell(std::string name, int damage, int manaCost = 3, int range = 2);
    
    void cast(Position target, GameField& field) override;
    std::string getName() const override { return name; }
    std::string getDescription() const override { return description; }
    int getManaCost() const override { return manaCost; }
    int getRange() const override { return range; }
    SpellType getType() const override { return SpellType::TRAP; }
};

#endif