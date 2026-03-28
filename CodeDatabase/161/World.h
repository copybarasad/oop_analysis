#ifndef WORLD_H
#define WORLD_H

#include "./managers/EntityManager.h"
#include "./managers/EnemyBuildingManager.h"
#include "./managers/EnemyTowerManager.h"
#include "./managers/PlayerManager.h"
#include "../map/Field.h"
#include "../entites/Enemy.h"
#include "../entites/Ally.h"

class World {
public:
	void Init(Field&, std::shared_ptr<Player>, int, int, int, int, int);
	void Update(Field&, std::shared_ptr<Player>);
	void DeleteDeadEntites(Field&);

	PlayerManager& PlayerMan();
	EntityManager<Enemy>& Enemies();
	EnemyBuildingManager& EnemiesBuildings();
	EnemyTowerManager& EnemiesTowers();
	EntityManager<Ally>& Allyes();
	TokenGameState SerializeState(Field& field, std::shared_ptr<Player> player);
	void LoadState(Field& field, std::shared_ptr<Player>& player, const TokenGameState& state);

	void SetLevel(int lvl) { level_ = lvl; }
	int GetLevel() const { return level_; }

private:
	int level_;

	EntityManager<Enemy> enemyManager_;
	EnemyBuildingManager buildingManager_;
	EnemyTowerManager towerManager_;
	EntityManager<Ally> allyManager_;
	PlayerManager playerManager_;
};

#endif // WORLD_H