//
// Created by Artem on 05.10.2025.
//

#ifndef LABA1_ENEMYSPAWNERBUILDINGMANAGER_H
#define LABA1_ENEMYSPAWNERBUILDINGMANAGER_H

#include "../../../Entities/Buildings/EnemySpawnerBuilding.h"
#include "../../../GameField/Field.h"
#include "../CreatureManagers/CompControlledCreatureManager.h"
#include "../EntityManager.h"

class EnemySpawnerBuildingManager : public EntityManager{
public:
    EnemySpawnerBuildingManager(Field &field, int stepCounter,
                                std::shared_ptr<EnemySpawnerBuilding> &building, EnemySpawner &enemySpawner)
            : EntityManager(field, building.get()), stepCounter(stepCounter), building(building), enemySpawner(enemySpawner) {}
    EnemySpawnerBuildingManager(Field &field, std::shared_ptr<EnemySpawnerBuilding> &building, EnemySpawner &enemySpawner)
            : EntityManager(field, building.get()), stepCounter(0), building(building), enemySpawner(enemySpawner) {}

    virtual bool isTimeToSpawn() noexcept final;
    virtual void resetSpawnCounter() noexcept final;
    virtual void incrementTimeToSpawnCounter() noexcept final;
    virtual void spawnEnemy() final;

protected:
    int stepCounter;
    std::shared_ptr<EnemySpawnerBuilding> building;
    EnemySpawner &enemySpawner;
};


#endif //LABA1_ENEMYSPAWNERBUILDINGMANAGER_H
