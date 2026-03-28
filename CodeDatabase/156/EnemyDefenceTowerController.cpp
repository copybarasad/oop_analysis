//
// Created by Artem on 30.10.2025.
//

#include "EnemyDefenceTowerController.h"
#include <iostream>
#include <cmath>
#include "../../GameMaster.h"
#include "../../../Logger/Logger.h"

void EnemyDefenceTowerController::doMove(GameMasterBase &gameMaster) {
    std::string headStr;
    {
        std::stringstream ss;
        ss << &this->manager;
        headStr = ss.str();
    }

    Logger::info("Ход, EnemyDefenceTower: " + headStr);

    if (this->manager.isCoolDowned()){

        std::vector<CreatureWithDistance> spottedEnemies;

        int attackRadius = this->manager.getAttackRadius();
        int minSpottedRadius = INT_MAX;
        int startX, endX, startY, endY;
        startX = this->manager.getEntityCoords().x - attackRadius >= 0 ? this->manager.getEntityCoords().x - attackRadius : 0;
        endX = this->manager.getEntityCoords().x + attackRadius < this->field.getHeight() ? this->manager.getEntityCoords().x + attackRadius : this->field.getHeight()-1;
        startY = this->manager.getEntityCoords().y - attackRadius >= 0 ? this->manager.getEntityCoords().y - attackRadius : 0;
        endY = this->manager.getEntityCoords().y + attackRadius < this->field.getWidth() ? this->manager.getEntityCoords().y + attackRadius : this->field.getWidth()-1;

        for (int x = startX; x < endX; ++x) {
            for (int y = startY; y < endY; ++y) {
                this->field.isCoordsAvailable(x, y);
                if (this->field.getFieldCells()[x][y].hasEntityInCell() && this->typesToAttackWithPriority.count(this->field.getFieldCells()[x][y].getEntityInCellType()) > 0){
                    int rad = std::max(abs(x - this->manager.getEntityCoords().x), abs(y - this->manager.getEntityCoords().y));
                    minSpottedRadius = std::min(minSpottedRadius, rad);
                    spottedEnemies.push_back(CreatureWithDistance{this->field.getFieldCells()[x][y].getEntityInCellType(), rad, {x, y}});
                }
            }
        }

        {
            std::vector<CreatureWithDistance> tmp;
            for(const CreatureWithDistance& creature : spottedEnemies){
                if (creature.distance == minSpottedRadius)
                    tmp.push_back(creature);
            }
            spottedEnemies = std::vector<CreatureWithDistance>(tmp);
        }

        if (!spottedEnemies.empty()){
            CreatureWithDistance priorityEnemy = spottedEnemies[0];

            for(const CreatureWithDistance& creature : spottedEnemies){
                if (this->typesToAttackWithPriority.at(priorityEnemy.creatureType) > this->typesToAttackWithPriority.at(priorityEnemy.creatureType)){
                    priorityEnemy = creature;
                }
            }

            this->manager.attackCoordinates(priorityEnemy.coords);
            this->manager.goToCoolDown();
            gameMaster.redraw();
        }else{
            Logger::info("EnemyDefenceTower: " + headStr + " no target to damage");
        }
    }else{
        Logger::info("EnemyDefenceTower: " + headStr + " on cool down");
        this->manager.doCoolDownStep();
    }
    Logger::tech("EnemyDefenceTower: " + headStr + " закончило ход");
    gameMaster.redraw();
}

bool EnemyDefenceTowerController::isAlive() {
    return this->manager.isAlive();
}
