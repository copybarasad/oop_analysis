#pragma once
#include <string>
#include "SpellBase.hpp"

class SummonSpell : public SpellBase {
public:
    SummonSpell(int allyHp, int allyDamage, int allyCount);

    std::string name() const override;
    int hotkey() const noexcept override;
    bool requiresTarget() const noexcept override;
    bool cast(SpellContext& ctx) override;

private:
    int allyHp_;
    int allyDamage_;
    int allyCount_;
};
