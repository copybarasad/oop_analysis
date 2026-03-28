#ifndef ENHANCE_SPELL_HPP_INCLUDED
#define ENHANCE_SPELL_HPP_INCLUDED

#include "spell.hpp"

class EnhanceSpell : public Spell {
public:
    EnhanceSpell(int d=1,int a=1,int t=2,int s=1)
        : dPlus_(d), aPlus_(a), tPlus_(t), sPlus_(s) {}

    std::string name() const override { return "enhance"; }
    TargetMode targeting() const override { return TargetMode::None; }
    int radius() const override { return 0; }

    bool cast(Hero&,
              Field&,
              std::vector<Enemy>&,
              std::vector<Spawner>&,
              BoostState& boosts,
              const Position&) override {
        boosts.directRadiusPlus += dPlus_;
        boosts.aoeSizePlus      += aPlus_;
        boosts.trapDamagePlus   += tPlus_;
        boosts.summonCountPlus  += sPlus_;
        boosts.stacks++;
        return true;
    }

private:
    int dPlus_, aPlus_, tPlus_, sPlus_;
};

#endif
