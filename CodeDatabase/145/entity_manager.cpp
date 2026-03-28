#include "entity_manager.h"
#include "player.h"
#include "game_field.h"
#include "enemy.h"
#include "ally.h"
#include "enemy_building.h"
#include "enemy_tower.h"
#include <algorithm>

EntityManager::EntityManager(std::vector<Enemy>& enemies,
                             std::vector<Ally>& allies,
                             std::vector<EnemyBuilding>& buildings,
                             std::vector<EnemyTower>& towers,
                             GameField& field,
                             Player& player)
    : enemies(enemies), allies(allies), buildings(buildings), towers(towers),
      field(field), player(player) {}

template<typename T>
void EntityManager::removeDestroyedEntities(std::vector<T>& entities) {
    entities.erase(
        std::remove_if(entities.begin(), entities.end(),
                      [](const T& entity) { return !entity.isAlive(); }),
        entities.end()
    );
}

void EntityManager::cleanupDestroyedEnemies() {
    removeDestroyedEntities(enemies);
}

void EntityManager::cleanupDestroyedAllies() {
    removeDestroyedEntities(allies);
}

void EntityManager::cleanupDestroyedBuildings() {
    removeDestroyedEntities(buildings);
}

void EntityManager::cleanupDestroyedTowers() {
    removeDestroyedEntities(towers);
}

void EntityManager::cleanupAllDestroyed() {
    cleanupDestroyedEnemies();
    cleanupDestroyedAllies();
    cleanupDestroyedBuildings();
    cleanupDestroyedTowers();
}

bool EntityManager::hasEnemiesRemaining() const {
    return !enemies.empty();
}

bool EntityManager::hasAlliesRemaining() const {
    return !allies.empty();
}

bool EntityManager::hasStructuresRemaining() const {
    return !buildings.empty() || !towers.empty();
}