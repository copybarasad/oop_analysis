#include "EntityManager.hpp"
#include "entities/Enemy.hpp"
#include "entities/EnemyBarracks.hpp"
#include "entities/Player.hpp"
#include "entities/EnemyTower.hpp"

Entity* EntityManager::operator[](int index) {
    auto it = infoMap.find(index);
    if (it != infoMap.end()) {
        return it->second.get();
    }
    return nullptr;
}


const Entity* EntityManager::operator[](int index) const {
    auto it = infoMap.find(index);
    if (it != infoMap.end()) {
        return it->second.get();
    }
    return nullptr;
}


void EntityManager::createEntity(std::unique_ptr<Entity> entity, int index) {
    auto it = infoMap.find(index);
    if (it == infoMap.end() || !it->second) {
        infoMap[index] = std::move(entity);
    }
}


void EntityManager::killEntity(int index) {
    if (infoMap[index]) {
        infoMap.erase(index);
    }
}


void EntityManager::changeEntityIndex(int oldIndex, int newIndex) {
    if (infoMap.find(newIndex) != infoMap.end()) {
        return;
    }
    auto it = infoMap.find(oldIndex);
    if (it == infoMap.end()) return;
    infoMap[newIndex] = std::move(it->second);
    infoMap.erase(it);
}


Entity* EntityManager::getEntity(int index) {
    auto it = infoMap.find(index);
    if (it == infoMap.end()) return nullptr;
    return it->second.get();
}


std::unique_ptr<Entity> EntityManager::returnEntity(int index) {
    return std::move(infoMap[index]);
}


std::vector<int> EntityManager::getIndexesWithEntity(Entity::entityType type) const {
    std::vector<int> indexesOfCellsWithEntity{};
    for (const auto& pair : infoMap) {
        if (pair.second->getType() == type) {
            indexesOfCellsWithEntity.push_back(pair.first);
        }
    }
    return indexesOfCellsWithEntity;
}


const std::unordered_map<int, std::unique_ptr<Entity>>& EntityManager::returnInfoMap() {
    return this->infoMap;
}


EntitySaveData EntityManager::getEntitySaveData() {
    EntitySaveData data;

    for (int index : getIndexesWithEntity(Entity::entityType::ENEMY)) {
        if (auto* enemy = dynamic_cast<Enemy*>(getEntity(index))) {
            EnemySaveData enemyData = enemy->getEnemySaveData();
            enemyData.enemyIndex = index;
            data.enemyData.push_back(enemyData);
        }
    }

    auto players = getIndexesWithEntity(Entity::entityType::PLAYER);
    if (!players.empty()) {
        int index = players[0];
        if (auto* player = dynamic_cast<Player*>(getEntity(index))) {
            data.playerData = player->getPlayerSaveData();
            data.playerData.playerIndex = index;
        }
    }
    else {
        data.playerData.playerIndex = -1;
    }

    auto towers = getIndexesWithEntity(Entity::entityType::TOWER);
    if (!towers.empty()) {
        int index = towers[0];
        if (auto* tower = dynamic_cast<EnemyTower*>(getEntity(index))) {
            data.towerData = tower->getTowerSaveData();
            data.towerData.towerIndex = index;
        }
    }
    else {
        data.towerData.towerIndex = -1;
    }

    auto barracks = getIndexesWithEntity(Entity::entityType::BARRACKS);
    if (!barracks.empty()) {
        int index = barracks[0];
        if (auto* bar = dynamic_cast<EnemyBarracks*>(getEntity(index))) {
            data.barrackData = bar->getBarrackSaveData();
            data.barrackData.barrackIndex = index;
        }
    }
    else {
        data.barrackData.barrackIndex = -1;
    }

    return data;
}


void EntityManager::setEntitySaveData(EntitySaveData data) {
    infoMap.clear();
    for (const auto& enemyData : data.enemyData) {
        auto enemy = std::make_unique<Enemy>();
        enemy->setEnemySaveData(enemyData);
        if (enemyData.enemyIndex >= 0) {
            if (getEntity(enemyData.enemyIndex) == nullptr) {
                createEntity(std::move(enemy), enemyData.enemyIndex);
            }
        }
    }
    auto player = std::make_unique<Player>();
    player->setPlayerSaveData(data.playerData);
    if (data.playerData.playerIndex >= 0) {
        createEntity(std::move(player), data.playerData.playerIndex);
    }
    auto tower = std::make_unique<EnemyTower>();
    tower->setTowerSaveData(data.towerData);
    if (data.towerData.towerIndex >= 0) {
        createEntity(std::move(tower), data.towerData.towerIndex);
    }
    auto barrack = std::make_unique<EnemyBarracks>();
    barrack->setBarrackSaveData(data.barrackData);
    if (data.barrackData.barrackIndex >= 0) {
        createEntity(std::move(barrack), data.barrackData.barrackIndex);
    }
    }
