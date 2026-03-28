#include "Entity.h"

Entity::Entity(int health, std::pair<int, int> pos, int damage)
	: health(health), pos(pos), damage(damage), isDied(false) {}

void Entity::Move(std::pair<int, int> steps)
{
	pos.first += steps.first;
	pos.second += steps.second;
}

void Entity::Attack(std::shared_ptr<Entity>){}

void Entity::TakeDamage(int receivedDamage)
{
	if (health == 0)
	{
		isDied = true;
		return;
	}

	if (health <= receivedDamage)
	{
		isDied = true;
		health = 0;
	}

	else
		health -= receivedDamage;
}

int Entity::GetHealth() const{
	return health;
}

int Entity::GetDamage() const {
	return damage;
}

std::pair<int, int> Entity::GetPosition() const{
	return pos;
}