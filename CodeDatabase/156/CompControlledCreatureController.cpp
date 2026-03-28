//
// Created by Artem on 23.10.2025.
//
#include <sstream>
#include "CompControlledCreatureController.h"
#include "../GameMasterBase.h"
#include "../../Logger/Logger.h"

void CompControlledCreatureController::computeAndDoMove(std::map<EntityType, bool>& typesToAttack, std::vector<EntityType>& priorityOfAttack, GameMasterBase& gameMaster) {
    std::string headerCout;
    {
        std::stringstream ss;
        ss << static_cast<const void*>(this);
        headerCout = (std::string("type: ") + (manager.getEntityType() == EntityType::EnemyEnt ? "enemy " : "ally ")) + ", id: " + ss.str();
    }

    Logger::tech("Ход CompControlledCreature: " + headerCout);

    if (manager.isCreatureDisabled()){
        Logger::info("CompControlledCreature: " + headerCout + " disabled");
        this->manager.enableCreature();
        gameMaster.redraw();
        return;
    }

    std::stack<Constants::XYPair> trip;

    std::vector<EntityCoordsWithNearestPositionToAttackFrom> nearestHostileEntitiesCoordinates;
    std::vector<std::vector<int>> distances(this->field.getHeight(), std::vector<int>(this->field.getWidth(), -1));
    std::queue<Constants::XYPair> queue;
    queue.push(this->manager.getEntityCoords());
    distances[manager.getEntityCoords().x][manager.getEntityCoords().y] = 0;

    while (!queue.empty()) {
        Constants::XYPair tempCoords = queue.front();
        queue.pop();

        std::vector<Constants::XYPair> nearEntitiesToAttack = this->field.hasNearEntityOfSomeTypesWithAChance(typesToAttack,
                                                                                                              tempCoords.x,
                                                                                                              tempCoords.y, this->manager.getChanceToDetectHostile());
        if (!nearEntitiesToAttack.empty()) {
            for (Constants::XYPair entityCoordinates: nearEntitiesToAttack) {
                if (distances[entityCoordinates.x][entityCoordinates.y] == -1) {
                    distances[entityCoordinates.x][entityCoordinates.y] = -2; //условный статус код чтобы туда больше некто не заглядывал и ничего не добавлял
                    nearestHostileEntitiesCoordinates.push_back({tempCoords, entityCoordinates});
                }
            }
        }

        for (Constants::dxdy dxdy: Constants::dxdys) {
            if (this->field.canMoveToOrSpawnOnNoExcept(tempCoords.x + dxdy.x, tempCoords.y + dxdy.y) && distances[tempCoords.x + dxdy.x][tempCoords.y + dxdy.y] == -1) {
                distances[tempCoords.x + dxdy.x][tempCoords.y + dxdy.y] = distances[tempCoords.x][tempCoords.y] + 1;
                queue.push({tempCoords.x + dxdy.x, tempCoords.y + dxdy.y});
            }
        }
    }


//    for (int i = 0; i < distances.size(); ++i) {
//        for (int j = 0; j < distances[0].size(); ++j) {
//            std::cout << distances[i][j] << " ";
//        }
//        std::cout << std::endl;
//    }

    std::string toShowInConsole;

    if (nearestHostileEntitiesCoordinates.empty()){
        int moveStepRange = rand() % (this->manager.getStepRange()); //если противник не нашёл взгялядом некаких целей, то он ходит на рандомную (от 0 до максимальной дальности) позицию
        std::vector<Constants::XYPair> availableCoordinates;
        for (int x = 0; x < distances.size(); ++x) { //добавляем все координаты до которых можем дойти исходя из дальности хода = moveStepRange
            for (int y = 0; y < distances[0].size(); ++y) {
                if (distances[x][y] == moveStepRange && this->field.canMoveToOrSpawnOnNoExcept(x, y)){
                    availableCoordinates.push_back({x, y});
                }
            }
        }

        if (availableCoordinates.empty()){
            Logger::info("CompControlledCreature " + headerCout + " некого не нашла, ходить тоже не хочет, завершает ход!");
            return;
        }

        Constants::XYPair priorityCoords = availableCoordinates.at(rand() % availableCoordinates.size());
        while(priorityCoords.x != manager.getEntityCoords().x && priorityCoords.y != manager.getEntityCoords().y){
            for(Constants::dxdy dxdy : Constants::dxdys){
                if (distances[priorityCoords.x + dxdy.x][priorityCoords.y + dxdy.y] == (distances[priorityCoords.x][priorityCoords.y] - 1)){
                    trip.push(priorityCoords);
                    priorityCoords.x= priorityCoords.x + dxdy.x;
                    priorityCoords.y = priorityCoords.y + dxdy.y;
                }
            }
        }
        try {
            moveTo(trip, headerCout, gameMaster);
        }catch (UniversalStringException& exp){
            Logger::info("CompControlledCreature " + headerCout + " попало на замедляющую клетку, завершает ход!");
            gameMaster.redraw();
            return;
        }
    }else{
        EntityCoordsWithNearestPositionToAttackFrom priorityEnemy = chooseByPriority(nearestHostileEntitiesCoordinates, distances, priorityOfAttack);
        bool canAttack = this->manager.getStepRange() >= distances[priorityEnemy.attackFromCoordinates.x][priorityEnemy.attackFromCoordinates.y];
        Constants::XYPair priorityEnemyAttackFromCoords = priorityEnemy.attackFromCoordinates;

        while(priorityEnemyAttackFromCoords.x != manager.getEntityCoords().x || priorityEnemyAttackFromCoords.y != manager.getEntityCoords().y){
            for(Constants::dxdy dxdy : Constants::dxdys){
                int nX = priorityEnemyAttackFromCoords.x + dxdy.x;
                int nY = priorityEnemyAttackFromCoords.y + dxdy.y;
                if (nX < 0 || nX >= this->field.getHeight() || nY < 0 || nY >= this->field.getWidth())
                    continue;
                if (distances[nX][nY] == (distances[priorityEnemyAttackFromCoords.x][priorityEnemyAttackFromCoords.y] - 1)){
                    if (this->manager.getStepRange() >= distances[priorityEnemyAttackFromCoords.x][priorityEnemyAttackFromCoords.y]){
                        trip.push(priorityEnemyAttackFromCoords);
                    }
                    priorityEnemyAttackFromCoords.x = nX;
                    priorityEnemyAttackFromCoords.y = nY;
                    break;
                }
            }
        }

        try {
            moveTo(trip, headerCout, gameMaster);
        }catch (SlowingCellNotification& exp){
            gameMaster.redraw();
            Logger::info("CompControlledCreature " + headerCout + " попало на замедляющую клетку, завершает ход!");
            gameMaster.redraw();
            return;
        }

        if (canAttack) {
            manager.attack(priorityEnemy.entityCoordinates);
            toShowInConsole = "CompControlledCreature " + headerCout +  " аттакует сущность по координатам " + std::to_string(priorityEnemy.entityCoordinates.x) + " " + std::to_string(priorityEnemy.entityCoordinates.y);
        }
    }


    if (!toShowInConsole.empty())
        Logger::info(toShowInConsole);
    Logger::tech("CompControlledCreature " + headerCout + " штатно завершае ход");
    gameMaster.redraw();
}

CompControlledCreatureController::EntityCoordsWithNearestPositionToAttackFrom
CompControlledCreatureController::chooseByPriority(std::vector<EntityCoordsWithNearestPositionToAttackFrom> &enemyWithNearestPositionToAttack, std::vector<std::vector<int>>& distances, std::vector<EntityType>& priorityOfAttack) {
    std::vector<EntityCoordsWithNearestPositionToAttackFrom> nearestEnemies;
    int minRange = INT_MAX;
    for (EntityCoordsWithNearestPositionToAttackFrom position : enemyWithNearestPositionToAttack) {
        minRange = distances[position.attackFromCoordinates.x][position.attackFromCoordinates.y] < minRange ? distances[position.attackFromCoordinates.x][position.attackFromCoordinates.y] : minRange;
    }
    for (EntityCoordsWithNearestPositionToAttackFrom position : enemyWithNearestPositionToAttack) {
        if (distances[position.attackFromCoordinates.x][position.attackFromCoordinates.y] == minRange)
            nearestEnemies.push_back(position);
    }

    for (EntityType highestPriority: priorityOfAttack) {
        for (EntityCoordsWithNearestPositionToAttackFrom coords: nearestEnemies) {
            if (this->field.getFieldCells()[coords.entityCoordinates.x][coords.entityCoordinates.y].getEntityInCellType() ==
                highestPriority) {
                return coords;
            }
        }
    }
    throw UnexpectedBehaviorException("unexpected behavior: chooseByPriority");
}

void CompControlledCreatureController::moveTo(std::stack<Constants::XYPair> trip, std::string headerCout, GameMasterBase& gameMaster) {
    while(!trip.empty()){
        Constants::XYPair stepTo = trip.top();
        trip.pop();
        Constants::dxdy howToMove = {stepTo.x - manager.getEntityCoords().x, stepTo.y - manager.getEntityCoords().y};
        std::stringstream ss;
        ss << "CompControlledCreature " << headerCout <<  " перемещается в x: " << stepTo.x << " y: " << stepTo.y;
        Logger::info(ss.str());
        manager.moveTo(howToMove);
        if(!trip.empty()){
            gameMaster.redraw();
        }
    }
}

bool CompControlledCreatureController::isAlive() {
    return this->manager.isAlive();
}
