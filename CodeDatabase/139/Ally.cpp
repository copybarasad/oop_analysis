#include "Ally.h"


Ally::Ally(int health, int damage, int speed, std::string name)
	: Entity(health, damage, speed, name) { }


Ally::Ally(int health, int damage, const Point& point, int speed, std::string name)
	: Entity(health, damage, speed, name)
{
	this->setPosition(point);
}


void Ally::attack(Entity& other)
{
	other.takeDamage(damage);
	health = health - damage < 0 ? 0 : health - damage;
}