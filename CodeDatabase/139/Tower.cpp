#include "Tower.h"


Tower::Tower(int health, int radius, unsigned int spawnTime)
	: health(health), radius(radius), spawnTime(spawnTime) { }


void Tower::to_json(nlohmann::json& js) const
{
	js["position"] = 
	{
		{"x", pos.X()},
		{"y", pos.Y()}
	};
	js["health"] = health;
	js["radius"] = radius;
	js["spawnTime"] = spawnTime;
}


void Tower::from_json(const nlohmann::json& js)
{
	if (js.contains("health"))
		health = js["health"];
	else
		throw std::runtime_error(u8"Ошибка считывания health");

	if (js.contains("radius"))
		radius = js["radius"];
	else
		throw std::runtime_error(u8"Ошибка считывания radius");

	if (js.contains("spawnTime"))
		spawnTime = js["spawnTime"];
	else
		throw std::runtime_error(u8"Ошибка считывания spawnTime");

	if (js.contains("position") && js["position"].contains("x") && js["position"].contains("y"))
	{
		pos = Point(
			js["position"]["x"],
			js["position"]["y"]
		);
	}
	else
		throw std::runtime_error(u8"Ошибка считывания position");
}


void Tower::setHealth(int health)
{
	this->health = health;
}


void Tower::setPosition(const Point& pos)
{
	this->pos = pos;
}


void Tower::setSpawnTime(unsigned int time)
{
	spawnTime = time;
}


void Tower::setRadius(int radius)
{
	this->radius = radius;
}


const Point& Tower::getPosition() const
{
	return pos;
}


void Tower::takeDamage(const int damage)
{
	health = health - damage <= 0 ? 0 : health - damage;
}


int Tower::getRadius() const
{
	return radius;
}


unsigned int Tower::getSpawnTime() const
{
	return spawnTime;
}


int Tower::getHealth() const
{
	return health;
}


bool Tower::isDestroyed() const
{
	return health <= 0;
}