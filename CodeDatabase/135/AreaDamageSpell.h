#ifndef AREA_DAMAGE_SPELL_H
#define AREA_DAMAGE_SPELL_H

#include "BaseTargetedSpell.h"

class AreaDamageSpell : public BaseTargetedSpell {
private:
	int damage;
	int area_size;

protected:
	bool castImpl(SpellData& data) override;

public:
	AreaDamageSpell(int damage_value, int spell_radius, int area = 2);
};

#endif
