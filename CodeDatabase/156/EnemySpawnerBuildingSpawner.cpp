//
// Created by Artem on 05.10.2025.
//

#include "EnemySpawnerBuildingSpawner.h"

EnemySpawnerBuildingManager *EnemySpawnerBuildingSpawner::spawn(int x, int y) {
    try {
        this->field.canMoveToOrSpawnOn(x, y);
        std::shared_ptr<EnemySpawnerBuilding> building = std::make_shared<EnemySpawnerBuilding>(EnemySpawnerBuilding(x, y,
                                                                                                                     this->model.healthPoint,
                                                                                                                     this->model.spawnInterval)
                                                                                                );
        this->field.getFieldCells()[x][y].addEntityInCell(building);
        return new EnemySpawnerBuildingManager(this->field, building, this->enemySpawner);
    }catch (CellImpassableNotification& ex){
        throw SpawnEntityException(std::string("Can't spawn EnemyDefenceTowerManager because: ") + ex.what());
    }catch (CoordinateException& ex){
        throw SpawnEntityException(std::string("Can't spawn cEnemyDefenceTowerManager because: ") + ex.what());
    }
}
