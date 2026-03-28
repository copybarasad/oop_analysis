#ifndef ENEMY_MANAGER_H
#define ENEMY_MANAGER_H

#include <vector>
#include "enemy.h"
#include "gametypes.h"

class EnemyMovementContext;
class GameEventBus;

class EnemyManager {
private:
    std::vector<Enemy> enemies;

public:
    void spawnInitialEnemies(Field& field, BuildingManager& buildingManager, Player& player, int count,
                             int enemyHealth = 30, int enemyDamage = 10, GameEventBus* eventBus = nullptr);
    void moveEnemies(Field& field, Player& player, const EnemyMovementContext& context, GameEventBus* eventBus = nullptr);
    bool attackEnemyAtPosition(int x, int y, Player& player, GameEventBus* eventBus = nullptr);
    void performRangedAttack(Field& field, Player& player, int directionX, int directionY, GameEventBus* eventBus = nullptr);
    bool isCellOccupiedByEnemy(int x, int y) const;
    const std::vector<Enemy>& getEnemies() const { return enemies; }
    std::vector<Enemy>& getEnemies() { return enemies; }
};

#endif