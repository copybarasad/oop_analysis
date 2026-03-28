#include "GameWorldFacade.h"
#include "Player.h"
#include "EnemyManager.h"
#include "BuildingManager.h"
#include "TowerManager.h"
#include "TrapManager.h"
#include "AllyManager.h"
#include "GameField.h"
#include "Trap.h"
#include "Ally.h"
#include <cmath>
#include <memory>

GameWorldFacade::GameWorldFacade(Player* player, EnemyManager* enemyManager,
                                BuildingManager* buildingManager, TowerManager* towerManager,
                                TrapManager* trapManager, AllyManager* allyManager,
                                GameField* field)
    : player_(player), enemyManager_(enemyManager), buildingManager_(buildingManager),
      towerManager_(towerManager), trapManager_(trapManager), allyManager_(allyManager),
      field_(field) {}

bool GameWorldFacade::dealDamageToEnemy(Pos pos, int damage) {
    if (!enemyManager_ || !enemyManager_->hasEnemyOnCell(pos.x, pos.y)) {
        return false;
    }
    
    Enemy& enemy = enemyManager_->getEnemyAt(pos.x, pos.y);
    enemy.takeDamage(damage);
    return true;
}

bool GameWorldFacade::dealDamageToBuilding(Pos pos, int damage) {
    if (!buildingManager_ || !buildingManager_->hasBuildingOnCell(pos.x, pos.y)) {
        return false;
    }
    
    BuildingInstance& buildingInst = buildingManager_->getBuildingAt(pos.x, pos.y);
    buildingInst.building->takeDamage(damage);
    return true;
}

bool GameWorldFacade::dealDamageToTower(Pos pos, int damage) {
    if (!towerManager_ || !towerManager_->hasTowerOnCell(pos.x, pos.y)) {
        return false;
    }
    
    EnemyTower& tower = towerManager_->getTowerAt(pos.x, pos.y);
    tower.takeDamage(damage);
    return true;
}

bool GameWorldFacade::placeTrap(Pos pos, int damage) {
    if (!trapManager_ || !field_) {
        return false;
    }
    
    if (!isValidPosition(pos) || !field_->isCellPassable(pos.x, pos.y)) {
        return false;
    }
    
    if (enemyManager_ && enemyManager_->hasEnemyOnCell(pos.x, pos.y)) {
        return false;
    }
    if (buildingManager_ && buildingManager_->hasBuildingOnCell(pos.x, pos.y)) {
        return false;
    }
    if (towerManager_ && towerManager_->hasTowerOnCell(pos.x, pos.y)) {
        return false;
    }
    if (allyManager_ && allyManager_->hasAllyOnCell(pos.x, pos.y)) {
        return false;
    }
    if (trapManager_->hasTrapOnCell(pos.x, pos.y)) {
        return false;
    }
    if (player_ && player_->getPosition().x == pos.x && player_->getPosition().y == pos.y) {
        return false;
    }
    
    auto trap = std::make_unique<Trap>(pos, damage);
    trapManager_->addTrap(std::move(trap));
    return true;
}

bool GameWorldFacade::summonAlly(Pos target, int count) {
    if (!allyManager_ || !field_ || !player_) {
        return false;
    }
    
    std::vector<Pos> spawnPositions = {target};
    
    std::vector<Pos> directions = {{0,1}, {1,0}, {0,-1}, {-1,0}, {1,1}, {-1,1}, {1,-1}, {-1,-1}};
    for (const auto& dir : directions) {
        spawnPositions.push_back({target.x + dir.x, target.y + dir.y});
    }
    
    int spawned = 0;
    
    for (const auto& pos : spawnPositions) {
        if (spawned >= count) break;
        
        if (!isValidPosition(pos) || !field_->isCellPassable(pos.x, pos.y)) {
            continue;
        }
        
        bool isEmpty = true;
        if (enemyManager_ && enemyManager_->hasEnemyOnCell(pos.x, pos.y)) isEmpty = false;
        if (buildingManager_ && buildingManager_->hasBuildingOnCell(pos.x, pos.y)) isEmpty = false;
        if (towerManager_ && towerManager_->hasTowerOnCell(pos.x, pos.y)) isEmpty = false;
        if (allyManager_->hasAllyOnCell(pos.x, pos.y)) isEmpty = false;
        if (trapManager_ && trapManager_->hasTrapOnCell(pos.x, pos.y)) isEmpty = false;
        if (player_->getPosition().x == pos.x && player_->getPosition().y == pos.y) isEmpty = false;
        
        if (isEmpty) {
            auto ally = std::make_unique<Ally>(30, 15);
            allyManager_->addAlly(std::move(ally), pos.x, pos.y);
            spawned++;
        }
    }
    
    return spawned > 0;
}

bool GameWorldFacade::isInRange(Pos from, Pos to, int range) const {
    int dist = std::abs(from.x - to.x) + std::abs(from.y - to.y);
    return dist <= range;
}

Pos GameWorldFacade::getPlayerPosition() const {
    if (!player_) {
        return {0, 0};
    }
    return player_->getPosition();
}

bool GameWorldFacade::isValidPosition(Pos pos) const {
    if (!field_) {
        return false;
    }
    return field_->isValidPosition(pos.x, pos.y);
}

