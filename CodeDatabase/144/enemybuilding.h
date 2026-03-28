#ifndef ENEMY_BUILDING_H
#define ENEMY_BUILDING_H

#include "damageable.h"

class EnemyBuilding : public Damageable {
private:
	int spawnInterval;
	int turnsUntilSpawn;
	int x;
	int y;
	int health;
	int maxHealth;

public:
	struct Snapshot {
		int spawnInterval = 0;
		int turnsUntilSpawn = 0;
		int positionX = -1;
		int positionY = -1;
		int health = 0;
		int maxHealth = 0;
	};

	EnemyBuilding(int spawnInterval = 5, int x = -1, int y = -1, int health = 100);
	
	virtual void update();
	bool shouldSpawnEnemy();
	void resetSpawnTimer();
	int getTurnsUntilSpawn() const;
	int getSpawnInterval() const;
	int getX() const;
	int getY() const;
	int getPositionX() const override;
	int getPositionY() const override;
	void setPosition(int newX, int newY);
	void setSpawnInterval(int value);
	void setTurnsUntilSpawn(int value);
	
	int getHealth() const override;
	int getMaxHealth() const override;
	void takeDamage(int damageAmount) override;
	bool isDestroyed() const;
	bool isAlive() const override;
	void setHealth(int value);
	void setMaxHealth(int value);
	Snapshot createSnapshot() const;
	static EnemyBuilding fromSnapshot(const Snapshot& snapshot);
};

#endif