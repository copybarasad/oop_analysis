#include "entity.h"

Entity::Entity(int health, int points) : health(health), points(points){}

int Entity::getHealth() const
{
	return health;
}

int Entity::getPoints() const
{
	return points;
}

void Entity::setHealth(int value)
{
	health = value;
}

void Entity::setPoints(int value)
{
	points = value;
}

void Entity::receivePoints(int value)
{
	points+=value;
}

void Entity::receiveDamage(int value)
{
	health-=value;
}