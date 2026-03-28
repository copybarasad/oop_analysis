#ifndef SUMMONSPELL_H
#define SUMMONSPELL_H

#include "Spell.h"

class AllyManager;

class SummonSpell : public Spell {
private:
    std::string name_;
    AllyManager* allyManager_;
    int summonBonus_;

public:
    SummonSpell(const std::string& name = "Summon Ally", AllyManager* am = nullptr);
    
    void setAllyManager(AllyManager* am) { allyManager_ = am; }
    
    bool cast(SpellContext& context, EnhancementState& enhancements) override;
    std::string getName() const override;
    std::string getDescription() const override;
    SpellType getType() const override;
    std::unique_ptr<Spell> clone() const override;
    void applyPermanentBonus(const EnhancementState& bonus) override;
};

#endif

