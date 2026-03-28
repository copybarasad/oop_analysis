#ifndef AREA_DAMAGE_SPELL_H
#define AREA_DAMAGE_SPELL_H

#include "Spell.h"
#include "GameField.h"

class AreaDamageSpell : public Spell {
private:
    int damage;
    int areaSize;
    int enhancedSize;

public:
	AreaDamageSpell(int dmg, int size);
	bool cast(GameField& field, int targetX, int targetY) override;
	void enhance() override;
	std::string getName() const override;
	SpellId getId() const override;
};

#endif