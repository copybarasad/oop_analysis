#ifndef DIRECT_DAMAGE_SPELL_H
#define DIRECT_DAMAGE_SPELL_H

#include "BaseTargetedSpell.h"

class DirectDamageSpell : public BaseTargetedSpell {
private:
	int damage;

protected:
	bool castImpl(SpellData& data) override;

public:
	DirectDamageSpell(int damage_value, int spell_radius);
};

#endif
