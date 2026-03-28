//
// Created by Artem on 09.10.2025.
//


#include "EnemySpawnerBuildingController.h"
#include "../../GameMasterBase.h"
#include "../../../Logger/Logger.h"

void EnemySpawnerBuildingController::doMove(GameMasterBase& gameMaster) {
    std::string headStr;
    {
        std::stringstream ss;
        ss << &this->manager;
        headStr = "id: " + ss.str() + ": ";
    }


    Logger::info("Ход, enemySpawnerBuilding: " + headStr);
    if (this->manager.isTimeToSpawn()){
        Logger::info("enemySpawnerBuilding: " + headStr + " создаёт нового противнка");
        manager.resetSpawnCounter();
        manager.spawnEnemy();
        gameMaster.redraw();
        Logger::tech("enemySpawnerBuilding: " + headStr + " новый противник успешно создан");
    }else{
        Logger::info("enemySpawnerBuilding: " + headStr + " находиться на cooldown");
        manager.incrementTimeToSpawnCounter();
    }
    Logger::tech("enemySpawnerBuilding: " + headStr + " закончило ход");
    gameMaster.redraw();
}

bool EnemySpawnerBuildingController::isAlive() {
    return this->manager.isAlive();
}
