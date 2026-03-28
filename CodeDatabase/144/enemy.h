#ifndef ENEMY_H
#define ENEMY_H

#include "damageable.h"
#include "enemy_action_context.h"

class Enemy : public Damageable {
private:
	int health;
	int maxHealth;
	int damage;
	int x;
	int y;

public:
	struct Snapshot {
		int health = 0;
		int maxHealth = 0;
		int damage = 0;
		int positionX = -1;
		int positionY = -1;
	};

	Enemy(int health = 30, int damage = 10, int x = -1, int y = -1);
	
	int getHealth() const override;
	int getMaxHealth() const override; 
	int getDamage() const;
	int getX() const;
	int getY() const;
	int getPositionX() const override;
	int getPositionY() const override;
	
	void takeDamage(int damageAmount) override;
	void setPosition(int newX, int newY);
	void setHealth(int value);
	void setMaxHealth(int value);
	void setDamageValue(int value);
	bool isAlive() const override;
	void onDefeated(EnemyActionContext& context) override;
	Snapshot createSnapshot() const;
	static Enemy fromSnapshot(const Snapshot& snapshot);
};

#endif