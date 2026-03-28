#ifndef DIRECTDAMAGESPELL_H
#define DIRECTDAMAGESPELL_H

#include "Spell.h"

class DirectDamageSpell : public Spell {
public:
	DirectDamageSpell(int damage, int radius);
	
	bool Cast(GameField& field, int targetX, int targetY) override;
	std::string GetName() const override;
	void Enhance() override;
};

#endif
