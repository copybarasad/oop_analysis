#ifndef ENEMY_H
#define ENEMY_H

#include "Entity.h"
#include "Player.h"

class Enemy: public Entity
{
public:
	Enemy(std::pair<int, int>, int=ENEMY_HEALTH, int=ENEMY_DAMAGE);
	void Attack(std::shared_ptr<Entity>) override;
};

#endif

