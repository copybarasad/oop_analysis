#pragma once
#include <string>
#include "SpellBase.hpp"

class AreaDamageSpell : public SpellBase {
public:
    AreaDamageSpell(int baseDamage, int areaSize, int baseRange);

    std::string name() const override;
    int hotkey() const noexcept override;
    bool requiresTarget() const noexcept override;
    bool cast(SpellContext& ctx) override;

private:
    int baseDamage_;
    int areaSize_;
    int baseRange_;
};
