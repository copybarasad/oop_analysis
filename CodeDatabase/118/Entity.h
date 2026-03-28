#ifndef ENTITY_H
#define ENTITY_H

#include <utility>
#include <iostream>
class Entity
{
protected:
	int health, damage;
	std::pair<int, int> pos;

public:
	bool isDied;
	Entity(int, std::pair<int, int>, int damage = 0);
	void Move(std::pair<int, int>);
	virtual void Attack(std::shared_ptr<Entity>);
	void TakeDamage(int);
	int GetHealth()const;
	int GetDamage()const;
	std::pair<int, int> GetPosition()const;
};

#endif

