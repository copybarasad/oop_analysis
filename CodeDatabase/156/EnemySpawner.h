//
// Created by Artem on 05.10.2025.
//

#ifndef LABA1_ENEMYSPAWNER_H
#define LABA1_ENEMYSPAWNER_H

#include "../model/CompControlledCreatureModel.h"
#include "../Entities/Creatures/CompControlledCreature.h"
#include "../GameField/Field.h"
#include "../GameField/Cell.h"
#include "../Exceptions/Notifications/CellImpassableNotification.h"
#include "../Exceptions/CoordinateException.h"
#include "../Exceptions/SpawnEntityException.h"
#include "../GameManagment/EntityManagers/CreatureManagers/CompControlledCreatureManager.h"
class GameMasterBase;


class EnemySpawner {
public:
    EnemySpawner(const CompControlledCreatureModel &enemyModel, Field &field, GameMasterBase& gameMaster) : enemyModel(enemyModel), field(field), gameMaster(gameMaster) {}

    void createEnemy(int x, int y);

protected:
    const CompControlledCreatureModel enemyModel;
    Field& field;
    GameMasterBase& gameMaster;
};


#endif //LABA1_ENEMYSPAWNER_H
