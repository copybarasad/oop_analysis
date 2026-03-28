//
// Created by Artem on 05.10.2025.
//

#include <iostream>
#include "EnemySpawnerBuildingManager.h"

bool EnemySpawnerBuildingManager::isTimeToSpawn() noexcept {
    return this->stepCounter == this->building->getSpawnInterval();
}

Constants::dxdy chooseDxDy(std::vector<Constants::dxdy> dxdys) {
    int ind = rand() % dxdys.size();
    Constants::dxdy pair = dxdys.at(ind);
    dxdys.erase(dxdys.begin() + ind);
    return pair;
}

void EnemySpawnerBuildingManager::spawnEnemy() {
    std::vector<Constants::dxdy> dxdyCopy = Constants::dxdys;

    while (!dxdyCopy.empty()) {
        try{
            Constants::dxdy pair = chooseDxDy(dxdyCopy);

           this->enemySpawner.createEnemy(this->building->getXCoordinate() + pair.x,
                                                  this->building->getYCoordinate() + pair.y);
           return;
        }catch (SpawnEntityException& ex){
            continue;
        }
    }
    throw SpawnEntityException("EnemySpawnerBuildingManager spawnEnemy(): enable to spawn compControlledCreature nearby");
}

void EnemySpawnerBuildingManager::incrementTimeToSpawnCounter() noexcept {
    this->stepCounter++;
}

void EnemySpawnerBuildingManager::resetSpawnCounter() noexcept {
    this->stepCounter = 0;
}


