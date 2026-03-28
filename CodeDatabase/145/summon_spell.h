#ifndef SUMMON_SPELL_H
#define SUMMON_SPELL_H

#include "spell.h"
#include "summon_context.h"
#include <string>

class SummonSpell : public Spell {
private:
    int summonCount;
    int summonHealth;
    int summonDamage;
    
public:
    SummonSpell(int summonCount = 1, int health = 15, int damage = 5);

    bool apply(const SummonContext& context);
    bool canUse(const SummonContext& context) const;
    
    const char* getName() const override;
    std::unique_ptr<Spell> clone() const override;
    void applyEnhancement() override;
    std::string getSerializedData() const override;

    
    int getSummonCount() const;
    void setSummonCount(int newCount);
};

#endif