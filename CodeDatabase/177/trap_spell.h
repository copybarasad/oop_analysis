#ifndef TRAP_SPELL_H
#define TRAP_SPELL_H

#include "spell_card.h"

class TrapSpell : public SpellCard {
private:
    std::string name;
    std::string description;
    int manaCost;
    int range;
    int damage;
    static const int DEFAULT_DAMAGE = 12;
    static const int DEFAULT_RANGE = 2;

public:
    TrapSpell(int spellDamage = DEFAULT_DAMAGE, int spellRange = DEFAULT_RANGE);
    
    std::string getName() const override;
    std::string getDescription() const override;
    int getManaCost() const override;
    int getRange() const override;
    bool requiresTarget() const override;
    void cast() override;
    bool canCast(const Position& casterPosition, const Position& targetPosition) const override;
    
    int getDamage() const;
};

#endif