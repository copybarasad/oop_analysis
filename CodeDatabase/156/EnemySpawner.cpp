//
// Created by Artem on 05.10.2025.
//

#include "EnemySpawner.h"
#include "../GameManagment/Controllers/CompControlledCreatureController.h"
#include "../GameManagment/GameMasterBase.h"
#include "../GameManagment/Controllers/EnemyController.h"

class CompControlledCreatureController;

#include <memory>


void EnemySpawner::createEnemy(int x, int y) {
    try{
        this->field.canMoveToOrSpawnOn(x, y);
        std::shared_ptr<CompControlledCreature> enemy = std::make_shared<CompControlledCreature> ( x, y, enemyModel.healthPoint, EntityType::EnemyEnt,
                                 false, enemyModel.damage, enemyModel.stepCount, enemyModel.chanceToDetectHostile);
        this->field.getFieldCells()[x][y].addEntityInCell(enemy);
        if(field.getFieldCells()[x][y].hasCellEvent()){
            field.getFieldCells()[x][y].impactOnCreatureByCellEvent();
        }
        CompControlledCreatureManager* manager = new CompControlledCreatureManager(this->field, enemy);
        EnemyController* controller = new EnemyController(this->field, *manager);
        this->gameMaster.addEnemyController(controller, enemy);
    }catch (CellImpassableNotification& ex){
        throw SpawnEntityException(std::string("Can't spawn compControlledCreature Enemy because: ") + ex.what());
    }catch (CoordinateException& ex){
        throw SpawnEntityException(std::string("Can't spawn compControlledCreature Enemy because: ") + ex.what());
    }
}
