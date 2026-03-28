#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include "enemy.h"
#include "ally.h"
#include "enemy_building.h"
#include "enemy_tower.h"
#include <vector>
#include <utility>

class Player;
class GameField;

class EntityManager {
private:
    std::vector<Enemy>& enemies;
    std::vector<Ally>& allies;
    std::vector<EnemyBuilding>& buildings;
    std::vector<EnemyTower>& towers;
    GameField& field;
    Player& player;

public:
    EntityManager(std::vector<Enemy>& enemies,
                  std::vector<Ally>& allies,
                  std::vector<EnemyBuilding>& buildings,
                  std::vector<EnemyTower>& towers,
                  GameField& field,
                  Player& player);

    std::vector<Enemy>& getEnemies() { return enemies; }
    std::vector<Ally>& getAllies() { return allies; }
    std::vector<EnemyBuilding>& getBuildings() { return buildings; }
    std::vector<EnemyTower>& getTowers() { return towers; }

    const std::vector<Enemy>& getEnemies() const { return enemies; }
    const std::vector<Ally>& getAllies() const { return allies; }
    const std::vector<EnemyBuilding>& getBuildings() const { return buildings; }
    const std::vector<EnemyTower>& getTowers() const { return towers; }

    void cleanupDestroyedEnemies();
    void cleanupDestroyedAllies();
    void cleanupDestroyedBuildings();
    void cleanupDestroyedTowers();
    void cleanupAllDestroyed();

    bool hasEnemiesRemaining() const;
    bool hasAlliesRemaining() const;
    bool hasStructuresRemaining() const;

private:
    template<typename T>
    void removeDestroyedEntities(std::vector<T>& entities);
};

#endif