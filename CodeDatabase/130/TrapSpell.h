#ifndef TRAPSPELL_H
#define TRAPSPELL_H

#include "Spell.h"

class TrapManager;

class TrapSpell : public Spell {
private:
    int baseDamage_;
    int baseRange_;
    std::string name_;
    TrapManager* trapManager_;

public:
    TrapSpell(int damage = 25, int range = 2, const std::string& name = "Bear Trap", TrapManager* tm = nullptr);
    
    void setTrapManager(TrapManager* tm) { trapManager_ = tm; }
    
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

