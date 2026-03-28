#ifndef ENTITY_H
#define ENTITY_H

class Player;
class Enemy;
class Building;
class Trap;
class Ally;

enum class EntityType { Player, Enemy, Building, Trap, Ally };

class Entity {
public:
	Entity(int id, int health);
	virtual ~Entity();
	int getId() const;
	int getHealth() const;
	void setHealth(int health);
	bool isAlive() const;
	virtual EntityType getType() const = 0;
	virtual int getDamage() const = 0;

	virtual Player* asPlayer();
	virtual Enemy* asEnemy();
	virtual Building* asBuilding();
	virtual Trap* asTrap();
	virtual Ally* asAlly();
	virtual class Tower* asTower();
protected:
	int id_;
	int health_;
};

#endif
