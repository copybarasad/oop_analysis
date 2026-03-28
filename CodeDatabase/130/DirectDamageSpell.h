#ifndef DIRECTDAMAGESPELL_H
#define DIRECTDAMAGESPELL_H

#include "Spell.h"

class DirectDamageSpell : public Spell {
private:
    int baseDamage_;
    int baseRange_;
    std::string name_;

public:
    DirectDamageSpell(int damage = 40, int range = 3, const std::string& name = "Fireball");
    
    bool cast(SpellContext& context, EnhancementState& enhancements) override;
    std::string getName() const override;
    std::string getDescription() const override;
    SpellType getType() const override;
    std::unique_ptr<Spell> clone() const override;
    void applyPermanentBonus(const EnhancementState& bonus) override;
    
private:
    bool isInRange(const Pos& from, const Pos& to, int range) const;
};

#endif

