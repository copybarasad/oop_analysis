//
// Created by Artem on 24.09.2025.
//

#ifndef LABA1_ENEMYSPAWNERBUILDING_H
#define LABA1_ENEMYSPAWNERBUILDING_H

#include "../../Factories/EnemySpawner.h"
#include "Building.h"

class EnemySpawnerBuilding : public Building {
public:
    EnemySpawnerBuilding(int xCoordinate, int yCoordinate, int healthPoint, int spawnInterval) : Building(xCoordinate, yCoordinate, healthPoint,
                                                     EntityType::EnemySpawnerBuildingEnt), spawnInterval(spawnInterval){}

    virtual int getSpawnInterval() noexcept final;

    std::unique_ptr<Entity> clone() const override;
    std::string serialize() override;
    static EnemySpawnerBuilding* deserialize(std::map<std::string, std::string> json) noexcept;

protected:
    int spawnInterval;
};


#endif //LABA1_ENEMYSPAWNERBUILDING_H
