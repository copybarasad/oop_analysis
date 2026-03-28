#include <iostream>
#include <format>
#include <stdexcept>
#include "entity.h"

#ifndef ENEMY_H
#define ENEMY_H

class Enemy : public Entity
{
private:
	int damage;
public:
	Enemy();
	Enemy(int health, int damage, int points);

	Enemy(const Enemy& other);
	Enemy& operator=(const Enemy& other);

	int getDamage() const;
	void setDamage(int value);

	void showStats() const;
};


#endif  // ENEMY_H
