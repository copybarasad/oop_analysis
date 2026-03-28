#ifndef SUMMON_CONTEXT_H
#define SUMMON_CONTEXT_H

#include <vector>

class Player;
class GameField;
class Ally;
class Enemy;
class EnemyBuilding;
class EnemyTower;

struct SummonContext {
    Player& player;
    GameField& field;
    std::vector<Enemy>& enemies;
    std::vector<EnemyBuilding>& buildings;
    std::vector<EnemyTower>& towers;
    std::vector<Ally>& allies;
    int targetX;
    int targetY;
};

#endif