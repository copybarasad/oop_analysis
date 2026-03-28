//
// Created by Artem on 05.10.2025.
//

#ifndef LABA1_ENEMYSPAWNERBUILDINGSPAWNER_H
#define LABA1_ENEMYSPAWNERBUILDINGSPAWNER_H

#include "../Model/EnemySpawnerBuildingModel.h"
#include "../GameField/Field.h"
#include "../GameManagment/EntityManagers/BuildingManagers/EnemySpawnerBuildingManager.h"
#include "EnemySpawner.h"

class EnemySpawnerBuildingSpawner {
public:
    EnemySpawnerBuildingSpawner(Field &field, const EnemySpawnerBuildingModel &model, EnemySpawner &enemySpawner)
            : field(field), model(model), enemySpawner(enemySpawner) {}

    EnemySpawnerBuildingManager* spawn(int x, int y);
private:
    Field& field;
    const EnemySpawnerBuildingModel model;
    EnemySpawner& enemySpawner;
};


#endif //LABA1_ENEMYSPAWNERBUILDINGSPAWNER_H
