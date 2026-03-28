#include "Enemy.h"

Enemy::Enemy(std::pair<int,int> pos, int health, int damage)
	: Entity(health, pos, damage){}
		

void Enemy::Attack(std::shared_ptr<Entity> target)
{
	target->TakeDamage(damage);
}
