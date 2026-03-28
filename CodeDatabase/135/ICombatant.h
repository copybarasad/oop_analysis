#ifndef ICOMBATANT_H
#define ICOMBATANT_H

#include "IEntity.h"

class ICombatant : public IEntity {
public:
	virtual int getDamage() const = 0;
};

#endif
