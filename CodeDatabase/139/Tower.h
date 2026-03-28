#pragma once
#include "DataTypes/Point.h"
#include "JsonLib/json.hpp"


class Tower
{
private:
	int radius;
	unsigned int spawnTime;
	Point pos;
	int health;
public:
	Tower(int health = 50, int radius=4, unsigned int spawnTime=20);

	void to_json(nlohmann::json& js) const;

	void from_json(const nlohmann::json& js);

	void setHealth(int health);

	void setPosition(const Point& pos);

	void setSpawnTime(unsigned int time);

	void setRadius(int radius);

	const Point& getPosition() const;

	void takeDamage(const int damage);

	int getRadius() const;

	unsigned int getSpawnTime() const;

	int getHealth() const;

	bool isDestroyed() const;
};