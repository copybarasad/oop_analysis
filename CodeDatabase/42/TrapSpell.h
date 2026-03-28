#pragma once
#include "Spell.h"
#include <string>

class TrapSpell : public Spell {
private:
    std::string name;
    int damage;
    int range;

public:
    TrapSpell(const std::string& spellName, int spellDamage, int spellRange);
    std::string getName() const override;
    std::string getDescription() const override;
    int getDamage() const override;
    int getRange() const override;
};
