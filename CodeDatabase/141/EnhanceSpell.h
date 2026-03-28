#ifndef ENHANCE_SPELL_H
#define ENHANCE_SPELL_H

#include "Spell.h"
#include "Enhancement.h"
#include <string>

class ISpellContext;

class EnhanceSpell : public Spell {
private:
    Enhancement enhancement;
    std::string name;
    std::string description;

public:
    EnhanceSpell(Enhancement enh);

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

    Enhancement getEnhancement() const;
};

#endif
