#pragma once
#include <string>
#include "SpellBase.hpp"

class EmpowerSpell : public SpellBase {
public:
    EmpowerSpell(int stackValue);
    std::string name() const override;
    int hotkey() const noexcept override;
    bool requiresTarget() const noexcept override;
    bool cast(SpellContext& ctx) override;

private:
    int stackValue_;
};
