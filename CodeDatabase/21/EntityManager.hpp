#pragma once
#include <unordered_map>
#include <memory>
#include <vector>
#include "entities/Entity.hpp"
#include "../core/GameContext.hpp"
#include "SaveData.hpp"

class EnemyTower;
class Player;
class Enemy;
class EnemyBarracks;


class EntityManager {
private:
    std::unordered_map<int, std::unique_ptr<Entity>> infoMap{};
public:
    EntityManager(){};
    EntitySaveData getEntitySaveData();
    void setEntitySaveData(EntitySaveData data);
    Entity* operator[](int index);
    const Entity* operator[](int index) const;
    void createEntity(std::unique_ptr<Entity> entity, int index);
    void killEntity(int index);
    void changeEntityIndex(int oldIndex, int newIndex);
    std::vector<int> getIndexesWithEntity(Entity::entityType type) const;
    Entity* getEntity(int index);
    std::unique_ptr<Entity> returnEntity(int index);
    const std::unordered_map<int, std::unique_ptr<Entity>>& returnInfoMap();
};
