#ifndef ENHANCESPELL_H
#define ENHANCESPELL_H

#include "Spell.h"

class EnhanceSpell : public Spell {
private:
    std::string name_;
    EnhancementState bonus_;

public:
    EnhanceSpell(const std::string& name = "Empower");
    
    bool cast(SpellContext& context, EnhancementState& enhancements) override;
    std::string getName() const override;
    std::string getDescription() const override;
    SpellType getType() const override;
    std::unique_ptr<Spell> clone() const override;
    void applyPermanentBonus(const EnhancementState& bonus) override;
};

#endif

