#pragma once
#include "DamageSpell.h"
#include "SpellCardVisitor.h"

class DirectSpell : public DamageSpell {
public:
	DirectSpell(double d, int r) : DamageSpell(d, r) {}
	bool apply(const SpellContext& context, GameArea& gamearea);
	std::string accept(const SpellCardVisitor& visitor) const override {
		return visitor.format(this);
	}
};