#ifndef TRAP_SPELL_H
#define TRAP_SPELL_H

#include "BaseTargetedSpell.h"

class TrapSpell : public BaseTargetedSpell {
private:
	int damage;

protected:
	bool castImpl(SpellData& data) override;

public:
	TrapSpell(int damage_value, int spell_radius);
};

#endif
