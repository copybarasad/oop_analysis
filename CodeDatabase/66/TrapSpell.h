#ifndef TRAPSPELL_H
#define TRAPSPELL_H

#include "Spell.h"

class TrapSpell : public Spell {
private:
    int baseDamage;
    int baseRange;

public:
    TrapSpell(int damage = 30, int range = 2);
    
    bool canCast(const Position& playerPos, const SpellTarget& target, const GameWorld& world) const override;
    void cast(const Position& playerPos, const SpellTarget& target, GameWorld& world, EnhancementBuffer& buffer) override;
    SpellType getType() const override;
    std::string getName() const override;
    std::string getDescription(const EnhancementBuffer& buffer) const override;
    
    int getBaseDamage() const;
    int getBaseRange() const;
};

#endif
