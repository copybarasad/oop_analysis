#ifndef ALLY_H
#define ALLY_H

#include "GameConstants.h"

class Ally {
private:
	int health;
	int damage;
	int lifetime;

public:
	Ally(int allyHealth = GameConstants::DEFAULT_ALLY_HEALTH,
		int allyDamage = GameConstants::DEFAULT_ALLY_DAMAGE);

	int getHealth() const;
	int getDamage() const;
	int getLifetime() const;
	void setLifetime(int value);
	void takeDamage(int damageTaken);
	bool isAlive() const;
	bool shouldDespawn() const;
	void updateLifetime();
};

#endif