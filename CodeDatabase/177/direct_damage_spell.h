#ifndef DIRECT_DAMAGE_SPELL_H
#define DIRECT_DAMAGE_SPELL_H

#include "spell_card.h"

class DirectDamageSpell : public SpellCard {
private:
    std::string name;
    std::string description;
    int manaCost;
    int range;
    int damage;
    static const int DEFAULT_DAMAGE = 15;
    static const int DEFAULT_RANGE = 3;

public:
    DirectDamageSpell(int spellDamage = DEFAULT_DAMAGE, int spellRange = DEFAULT_RANGE);
    
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