#ifndef AREA_DAMAGE_SPELL_H
#define AREA_DAMAGE_SPELL_H

#include "Spell.h"

class AreaDamageSpell : public Spell {
private:
    int damage;
    int range;
    int areaSize;
    int manaCost;
    std::string name;
    std::string description;

public:
    AreaDamageSpell(int spellDamage, int spellRange, int spellAreaSize, int cost);
    
    bool cast(GameManager& gameManager, int targetX, int targetY) override;
    std::string getName() const override;
    std::string getDescription() const override;
    int getManaCost() const override;
    bool requiresTarget() const override;
    
    int getDamage() const;
    int getRange() const;
    int getAreaSize() const;
};

#endif
