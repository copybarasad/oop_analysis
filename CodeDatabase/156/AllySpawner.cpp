//
// Created by Artem on 23.10.2025.
//

#include "AllySpawner.h"
#include "../GameManagment/EntityManagers/CreatureManagers/CompControlledCreatureManager.h"
#include "../GameManagment/Controllers/AllyController.h"
#include "../GameManagment/GameMasterBase.h"

void AllySpawner::createAlly(int x, int y) {
    try{
        this->field.canMoveToOrSpawnOn(x, y);
        std::shared_ptr<CompControlledCreature> ally = std::make_shared<CompControlledCreature>(CompControlledCreature(x, y, allyModel.healthPoint, EntityType::Ally,
                                                                                                                       false, allyModel.damage, allyModel.stepCount, allyModel.chanceToDetectHostile));
        this->field.getFieldCells()[x][y].addEntityInCell(ally);
        if(field.getFieldCells()[x][y].hasCellEvent()){
            field.getFieldCells()[x][y].impactOnCreatureByCellEvent();
        }

        CompControlledCreatureManager* manager = new CompControlledCreatureManager(this->field, ally);
        AllyController* controller = new AllyController(this->field, *manager);
        this->gameMaster.addAllyController(controller, ally);

    }catch (CellImpassableNotification& ex){
        throw SpawnEntityException(std::string("Can't spawn compControlledCreature Ally because: ") + ex.what());
    }catch (CoordinateException& ex){
        throw SpawnEntityException(std::string("Can't spawn compControlledCreature Ally because: ") + ex.what());
    }
}
