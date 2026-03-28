#ifndef ENTITY_H
#define ENTITY_H

class Entity
{
protected:
	int health;
	int points;
public:
	Entity() = default;
	Entity(int health, int points);
	int getHealth() const;
	int getPoints() const;
	void setHealth(int value);
	void setPoints(int value);

	void receivePoints(int value);
	void receiveDamage(int value);

	~Entity() = default;
};

#endif