#ifndef ALLY_H
#define ALLY_H

#include "ICombatant.h"
#include "Health.h"
#include "Attack.h"

class Ally : public ICombatant {
private:
	Attack ally_attack;
	Health ally_health;

public:
	Ally(int attack, int hp);
	
	EntityType getType() const override;
	void causeDamage(int damage) override;
	int getDamage() const override;
	std::pair<int, int> getHealth() const override;
	bool alive() const override;
	std::unique_ptr<IEntity> clone() const override;
};

#endif
