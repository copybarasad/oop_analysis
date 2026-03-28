#ifndef ENEMY_BARRACKS_H
#define ENEMY_BARRACKS_H

#include "IEntity.h"
#include "ISpawner.h"
#include "Health.h"

class EnemyBarracks : public IEntity, public ISpawner {
private:
	Health barracks_health;
	int spawn_period;
	int counter;

	void decrementCounter();
	bool readyToSpawn() const;
	void resetCounter();

public:
	EnemyBarracks(int hp, int period);
	
	EntityType getType() const override;
	void causeDamage(int damage) override;
	std::pair<int, int> getHealth() const override;
	bool alive() const override;
	std::unique_ptr<IEntity> clone() const override;
	
	int timeToSpawn() const override;
	bool update();
};

#endif 
