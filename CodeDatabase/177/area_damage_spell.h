#ifndef AREA_DAMAGE_SPELL_H
#define AREA_DAMAGE_SPELL_H

#include "spell_card.h"

class AreaDamageSpell : public SpellCard {
private:
    std::string name;
    std::string description;
    int manaCost;
    int range;
    int damage;
    static const int DEFAULT_DAMAGE = 10;
    static const int DEFAULT_RANGE = 2;
    static const int AREA_SIZE = 2;

public:
    AreaDamageSpell(int spellDamage = DEFAULT_DAMAGE, int spellRange = DEFAULT_RANGE);
    
    std::string getName() const override;
    std::string getDescription() const override;
    int getManaCost() const override;
    int getRange() const override;
    bool requiresTarget() const override;
    void cast() override;
    bool canCast(const Position& casterPosition, const Position& targetPosition) const override;
    
    int getDamage() const;
    int getAreaSize() const;
};

#endif