#ifndef TRAP_SPELL_H
#define TRAP_SPELL_H

#include "Spell.h"

class TrapSpell : public Spell {
private:
    int damage;
    int range;
    int manaCost;
    std::string name;
    std::string description;

public:
    TrapSpell(int spellDamage, int spellRange, int cost);
    
    bool cast(GameManager& gameManager, int targetX, int targetY) override;
    std::string getName() const override;
    std::string getDescription() const override;
    int getManaCost() const override;
    bool requiresTarget() const override;
    
    int getDamage() const;
    int getRange() const;
};

#endif
