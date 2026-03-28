#ifndef ENEMYBUILDINGMANAGER_H
#define ENEMYBUILDINGMANAGER_H

#include <vector>
#include <memory>
#include "../../entites/EnemyBuilding.h"
#include "../../entites/Enemy.h"
#include "../../map/Field.h"

class World;

class EnemyBuildingManager {
public:
	bool SpawnEnemiesBuildings(Field&, int, int);
	bool EnemiesBuildingTurn(Field&, World&);
	const std::vector<std::weak_ptr<EnemyBuilding>>& GetEnemiesBuildings() const;
	void DeleteDeadEnemiesBuildings(Field&);
	void AddEntity(std::weak_ptr<EnemyBuilding> building);


private:
	std::vector<std::weak_ptr<EnemyBuilding>> enemies_buildings_;
};

#endif // ENEMYBUILDINGMANAGER_H