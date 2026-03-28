#ifndef ENEMY_MANAGER_H
#define ENEMY_MANAGER_H

#include "enemy.h"
#include "game_field.h"
#include "player.h"
#include <vector>
#include <utility>

class Ally;
class EnemyBuilding;
class EnemyTower;

class EnemyManager {
private:
    std::vector<Enemy>& enemies;
    GameField& field;
    Player& player;
    std::vector<Ally>& allies;

public:
    EnemyManager(std::vector<Enemy>& enemies, GameField& field, Player& player, std::vector<Ally>& allies);

    void moveEnemies(const std::vector<EnemyBuilding>& buildings, const std::vector<EnemyTower>& towers);
    bool hasEnemiesRemaining() const;
    void processBuildingSpawns(std::vector<EnemyBuilding>& buildings,
                               const std::vector<EnemyTower>& towers);

private:
    bool isEnemyStillAlive(const Enemy& enemy) const;
    void checkTrapsAtPosition(Enemy& enemy);
};

#endif


