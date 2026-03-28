#pragma once
#include <string>
#include "SpellBase.hpp"

class TrapSpell : public SpellBase {
public:
    TrapSpell(int trapDamage, int placementRange, int trapLifetime);

    std::string name() const override;
    int hotkey() const noexcept override;
    bool requiresTarget() const noexcept override;
    bool cast(SpellContext& ctx) override;

private:
    int trapDamage_;
    int placementRange_;
    int trapLifetime_;
};
