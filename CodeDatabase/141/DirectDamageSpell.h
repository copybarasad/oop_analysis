#ifndef DIRECT_DAMAGE_SPELL_H
#define DIRECT_DAMAGE_SPELL_H

#include "Spell.h"
#include <string>

class DirectDamageSpell : public Spell {
private:
    int damage;
    int range;
    std::string name;
    std::string description;

public:
    DirectDamageSpell(int damage = 25, int range = 3);

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
