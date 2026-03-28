#ifndef DIRECT_DAMAGE_SPELL_H
#define DIRECT_DAMAGE_SPELL_H

#include "Spell.h"

class DirectDamageSpell : public Spell {
private:
    int damage;
    int range;
    int manaCost;
    std::string name;
    std::string description;

public:
    DirectDamageSpell(int spellDamage, int spellRange, int cost);
    
    bool cast(GameManager& gameManager, int targetX, int targetY) override;
    std::string getName() const override;
    std::string getDescription() const override;
    int getManaCost() const override;
    bool requiresTarget() const override;
    
    int getDamage() const;
    int getRange() const;
};

#endif
