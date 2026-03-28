#ifndef SUMMON_SPELL_H
#define SUMMON_SPELL_H

#include "Spell.h"
#include <string>

class SummonSpell : public Spell {
private:
    int summonCount;
    std::string name;
    std::string description;

public:
    SummonSpell(int count = 1);

    bool cast(ISpellContext &context) override;

    std::string getName() const override;

    std::string getDescription() const override;

    SpellType getType() const override;

    bool enhance(const Enhancement &enhancement) override;

    Spell *clone() const override;

    Spell *upgradedCopy() const override;

    bool requiresTarget() const override;

    bool castAt(ISpellContext &context, int targetX, int targetY) override;

    int getRange() const override { return 0; }

    void serialize(std::ostream &out) const override;

    int getSummonCount() const;
};

#endif
