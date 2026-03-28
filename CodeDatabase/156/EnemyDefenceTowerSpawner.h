//
// Created by Artem on 17.11.2025.
//

#ifndef LABAOOP2_ENEMYDEFENCETOWERSPAWNER_H
#define LABAOOP2_ENEMYDEFENCETOWERSPAWNER_H

#include "../Model/EnemyDefenceTowerModel.h"
#include "../GameField/Field.h"
#include "../GameManagment/EntityManagers/BuildingManagers/EnemyDefenceTowerManager.h"

class EnemyDefenceTowerSpawner {
public:
    EnemyDefenceTowerSpawner(Field &field, const EnemyDefenceTowerModel &model) : field(field), model(model) {}

    EnemyDefenceTowerManager* spawn(int x, int y);

private:
    Field& field;
    const EnemyDefenceTowerModel model;
};


#endif //LABAOOP2_ENEMYDEFENCETOWERSPAWNER_H
