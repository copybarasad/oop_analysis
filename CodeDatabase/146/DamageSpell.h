#pragma once
#include "SpellCard.h"
#include "vector"
#include "string"

class DamageSpell : public SpellCard {
protected: 
	double damage;
	int radius;
public:
	DamageSpell(double d, int r) : damage(d), radius(r) {}
	virtual bool apply(const SpellContext& context, GameArea& gamearea) = 0;
	virtual std::string accept(const SpellCardVisitor& visitor) const = 0;
};