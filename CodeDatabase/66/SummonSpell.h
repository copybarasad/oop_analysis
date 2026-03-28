#ifndef SUMMONSPELL_H
#define SUMMONSPELL_H

#include "Spell.h"

class SummonSpell : public Spell {
private:
    int baseSummonCount;

public:
    SummonSpell(int summonCount = 1);
    
    bool canCast(const Position& playerPos, const SpellTarget& target, const GameWorld& world) const override;
    void cast(const Position& playerPos, const SpellTarget& target, GameWorld& world, EnhancementBuffer& buffer) override;
    SpellType getType() const override;
    std::string getName() const override;
    std::string getDescription(const EnhancementBuffer& buffer) const override;
    
    int getBaseSummonCount() const;
};

#endif
