#pragma once
#include "Entity.h"
#include "JsonLib/json.hpp"


class Ally : public Entity
{
public:
	Ally(int health = 30, int damage = 5, int speed = 1, std::string name = "Ally");
	Ally(int health, int damage, const Point& point, int speed = 1, std::string name = "Ally");
	void attack(Entity& other);
};