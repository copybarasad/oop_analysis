#include "Entity.h"


int Entity::nextId = 1;

Entity::Entity(int health, int damage, unsigned int speed, std::string name)
	: health(health), damage(damage), speed(speed), name(name), id(nextId++) { }


void Entity::to_json(nlohmann::json& js) const
{
	js["name"] = name;
	js["position"] =
	{
		{"x", pos.X()},
		{"y", pos.Y()}
	};
	js["health"] = health;
	js["damage"] = damage;
	js["speed"] = speed;
}


void Entity::from_json(const nlohmann::json& js)
{
	if (js.contains("name"))
		name = js["name"];
	else
		throw std::runtime_error(u8"Отсутствует поле name");

	if (js.contains("health"))
		health = js["health"];
	else
		throw std::runtime_error(u8"Отсутствует поле health");

	if (js.contains("damage"))
		damage = js["damage"];
	else
		throw std::runtime_error(u8"Отсутствует поле damage");

	if (js.contains("speed"))
		speed = js["speed"];
	else
		throw std::runtime_error(u8"Отсутствует поле speed");

	if (js.contains("position") && js["position"].contains("x") && js["position"].contains("y"))
	{
		pos = Point(
			js["position"]["x"],
			js["position"]["y"]
		);
	}
	else
		throw std::runtime_error(u8"Отсутствует поле position");
}


void Entity::setPosition(const Point& pos)
{
	this->pos = pos;
}


void Entity::setSpeed(const int speed)
{
	this->speed = speed;
}


void Entity::setDamage(const int damage)
{
	this->damage = damage;
}


void Entity::setHealth(const int health)
{
	this->health = health;
}


int Entity::getId() const
{
	return id;
}


const Point& Entity::getPosition() const
{
	return pos;
}


int Entity::getSpeed() const
{
	return speed;
}


int Entity::getDamage() const
{
	return damage;
}


int Entity::getHealth() const
{
	return health;
}


bool Entity::isAlive() const
{
	return health > 0;
}


void Entity::takeDamage(const int damage)
{
	health = health - damage > 0 ? health - damage : 0;
}


void Entity::move(const Point& vector)
{
	pos += vector;
}


void Entity::attack(Entity& other) const
{
	other.health = other.health - damage < 0 ? 0 : other.health - damage;
}