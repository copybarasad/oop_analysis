#ifndef DIRECT_DAMAGE_CONTEXT_H
#define DIRECT_DAMAGE_CONTEXT_H

#include <vector>
#include <memory>

class Player;
class GameField;
class Enemy;
class EnemyBuilding;
class EnemyTower;

struct DirectDamageContext {
    Player& player;
    GameField& field;
    std::vector<Enemy>& enemies;
    std::vector<EnemyBuilding>& buildings;
    std::vector<EnemyTower>& towers;
    int targetX;
    int targetY;
};

#endif