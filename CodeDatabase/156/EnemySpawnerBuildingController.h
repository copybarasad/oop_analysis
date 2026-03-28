//
// Created by Artem on 09.10.2025.
//

#ifndef LABA1_ENEMYSPAWNERBUILDINGCONTROLLER_H
#define LABA1_ENEMYSPAWNERBUILDINGCONTROLLER_H
#include "../../EntityManagers/BuildingManagers/EnemySpawnerBuildingManager.h"
#include <iostream>
#include <utility>
#include "../EnemyController.h"

class GameMasterBase;

class EnemySpawnerBuildingController {
public:
    EnemySpawnerBuildingController(EnemySpawnerBuildingManager manager, const Field &field) : manager(std::move(manager)),
                                                                                               field(field) {}
    void doMove(GameMasterBase& gameMaster);
    bool isAlive();

protected:
    EnemySpawnerBuildingManager manager;
    const Field& field;
};


#endif //LABA1_ENEMYSPAWNERBUILDINGCONTROLLER_H
