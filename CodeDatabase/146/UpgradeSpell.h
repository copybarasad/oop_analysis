#pragma once
#include "SpellCard.h"
#include "SpellCardVisitor.h"

class UpgradeSpell : public SpellCard {
public:
    UpgradeSpell() {}

    bool apply(const SpellContext& context, GameArea& gamearea) override;
    std::string accept(const SpellCardVisitor& visitor) const override {
        return visitor.format(this);
    }
};
