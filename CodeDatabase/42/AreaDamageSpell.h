#pragma once
#include "Spell.h"
#include <string>

class AreaDamageSpell : public Spell {
private:
    std::string name;
    int damage;
    int range;
    int areaSize;

public:
    AreaDamageSpell(const std::string& spellName, int spellDamage, int spellRange, int areaSize);
    std::string getName() const override;
    std::string getDescription() const override;
    int getDamage() const override;
    int getRange() const override;
    int getAreaSize() const;
};
