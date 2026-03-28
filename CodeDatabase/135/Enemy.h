#ifndef ENEMY_H
#define ENEMY_H

#include "ICombatant.h"
#include "Health.h"
#include "Attack.h"

class Enemy : public ICombatant {
private:
	Attack enemy_attack;
	Health enemy_health;

public:
	Enemy(int attack, int hp);
	
	EntityType getType() const override;
	void causeDamage(int damage) override;
	int getDamage() const override;
	std::pair<int, int> getHealth() const override;
	bool alive() const override;
	std::unique_ptr<IEntity> clone() const override;
};

#endif 
