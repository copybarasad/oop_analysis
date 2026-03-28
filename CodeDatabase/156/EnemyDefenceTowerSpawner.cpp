//
// Created by Artem on 17.11.2025.
//

#include "EnemyDefenceTowerSpawner.h"
#include "../Exceptions/Notifications/CellImpassableNotification.h"
#include "../Exceptions/CoordinateException.h"
#include "../Exceptions/SpawnEntityException.h"
#include "../Entities/Creatures/Attacks/Spels/DirectDamageSpell.h"

EnemyDefenceTowerManager* EnemyDefenceTowerSpawner::spawn(int x, int y) {
    try{
        this->field.canMoveToOrSpawnOn(x, y);
        std::shared_ptr<EnemyDefenceTower> tower = std::make_shared<EnemyDefenceTower>(EnemyDefenceTower(x, y, this->model.healthPoint, this->model.attackRadius, this->model.attackInterval));
        this->field.getFieldCells()[x][y].addEntityInCell(tower);
        return new EnemyDefenceTowerManager(this->field, tower);
    }catch (CellImpassableNotification& ex){
        throw SpawnEntityException(std::string("Can't spawn EnemyDefenceTowerManager because: ") + ex.what());
    }catch (CoordinateException& ex){
        throw SpawnEntityException(std::string("Can't spawn cEnemyDefenceTowerManager because: ") + ex.what());
    }



}
