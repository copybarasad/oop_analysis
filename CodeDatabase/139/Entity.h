#pragma once
#include "DataTypes/Point.h"
#include "JsonLib/json.hpp"
#include <string>


class Entity
{
private:
	static int nextId;
protected:
	std::string name;
	int health;
	unsigned int speed;
	int damage;
	Point pos;
	int id;
public:	
	Entity(int health = 10, int damage = 20, unsigned int speed = 1, std::string name = "Bot");

	void to_json(nlohmann::json& js) const;

	void from_json(const nlohmann::json& js);

	void setPosition(const Point& pos);

	void setSpeed(const int speed);

	void setDamage(const int damage);

	void setHealth(const int health);

	int getId() const;

	const Point& getPosition() const;

	int getSpeed() const;

	int getDamage() const;

	int getHealth() const;

	bool isAlive() const;

	void takeDamage(const int damage);

	virtual void move(const Point& vector);

	void attack(Entity& other) const;

	virtual ~Entity() = default;
};