#pragma once
#include "Entity.h"
#include "struct/dataEnemy.h"

class Enemy : public Entity {
public:
	Enemy();
	Enemy(int hp, int dmg);



    Enemy(const Enemy& other) = default;
    Enemy(Enemy&& other) noexcept = default;
	dataEnemy Save();

	~Enemy() override;
};
