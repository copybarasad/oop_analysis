#ifndef AREADAMAGESPELL_H
#define AREADAMAGESPELL_H

#include "Spell.h"

class AreaDamageSpell : public Spell {
private:
    int baseDamage_;
    int baseRange_;
    int baseAreaSize_;
    std::string name_;

public:
    AreaDamageSpell(int damage = 20, int range = 4, int areaSize = 2, const std::string& name = "Meteor");
    
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

