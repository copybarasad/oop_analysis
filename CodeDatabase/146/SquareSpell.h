#pragma once
#include "DamageSpell.h"
#include "SpellCardVisitor.h"

class SquareSpell : public DamageSpell {
public:
	SquareSpell(double d) : DamageSpell(d, 2) {}
	bool apply(const SpellContext& context, GameArea& gamearea) override;
	std::string accept(const SpellCardVisitor& visitor) const override {
		return visitor.format(this);
	}
};