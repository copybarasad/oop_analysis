#ifndef AREADAMAGESPELL_H
#define AREADAMAGESPELL_H

#include "Spell.h"

class AreaDamageSpell : public Spell {
public:
	AreaDamageSpell(int damage, int radius, int areaSize);
	
	bool Cast(GameField& field, int targetX, int targetY) override;
	std::string GetName() const override;
	void Enhance() override;

private:
	int baseAreaSize;
	int currentAreaSize;
};

#endif
