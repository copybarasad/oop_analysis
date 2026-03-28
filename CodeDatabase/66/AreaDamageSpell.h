#ifndef AREADAMAGESPELL_H
#define AREADAMAGESPELL_H

#include "Spell.h"

class AreaDamageSpell : public Spell {
private:
    int baseDamage;
    int baseRange;
    int baseAreaSize;

public:
    AreaDamageSpell(int damage = 15, int range = 4, int areaSize = 2);
    
    bool canCast(const Position& playerPos, const SpellTarget& target, const GameWorld& world) const override;
    void cast(const Position& playerPos, const SpellTarget& target, GameWorld& world, EnhancementBuffer& buffer) override;
    SpellType getType() const override;
    std::string getName() const override;
    std::string getDescription(const EnhancementBuffer& buffer) const override;
    
    int getBaseDamage() const;
    int getBaseRange() const;
    int getBaseAreaSize() const;
};

#endif
