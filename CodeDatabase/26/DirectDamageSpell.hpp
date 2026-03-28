#pragma once
#include <optional>
#include <string>
#include "SpellBase.hpp"
#include "Position.hpp"

class DirectDamageSpell : public SpellBase {
public:
    DirectDamageSpell(int baseDamage,
                      int baseRange,
                      bool useStacks = true,
                      std::optional<Position> originOverride = std::nullopt);
    std::string name() const override;
    int hotkey() const noexcept override;
    bool requiresTarget() const noexcept override;
    bool cast(SpellContext& ctx) override;

private:
    int  baseDamage_;
    int  baseRange_;
    bool useStacks_;
    bool hasOriginOverride_;
    Position originOverride_;
};
