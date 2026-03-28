#ifndef SUMMON_SPELL_H
#define SUMMON_SPELL_H

#include "Spell.h"

class SummonSpell : public Spell {
private:
    int summonCount;
    int range;
    int manaCost;
    std::string name;
    std::string description;

public:
    SummonSpell(int count, int spellRange, int cost);
    
    bool cast(GameManager& gameManager, int targetX, int targetY) override;
    std::string getName() const override;
    std::string getDescription() const override;
    int getManaCost() const override;
    bool requiresTarget() const override;
    
    int getSummonCount() const;
    int getRange() const;
};

#endif
