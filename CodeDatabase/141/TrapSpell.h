#ifndef TRAP_SPELL_H
#define TRAP_SPELL_H

#include "Spell.h"
#include <string>

class TrapSpell : public Spell {
private:
    int damage;
    int range;
    std::string name;
    std::string description;

public:
    TrapSpell(int trapDamage = 25, int trapRange = 1);

    bool cast(ISpellContext &context) override;

    bool castAt(ISpellContext &context, int targetX, int targetY) override;

    std::string getName() const override;

    std::string getDescription() const override;

    SpellType getType() const override;

    int getRange() const override;

    bool enhance(const Enhancement &enhancement) override;

    Spell *clone() const override;

    Spell *upgradedCopy() const override;

    bool requiresTarget() const override;

    int getDamage() const;

    void serialize(std::ostream &out) const override;
};

#endif
