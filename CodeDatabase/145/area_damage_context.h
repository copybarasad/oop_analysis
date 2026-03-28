#ifndef AREA_DAMAGE_CONTEXT_H
#define AREA_DAMAGE_CONTEXT_H

#include <vector>

class Player;
class GameField;
class Enemy;
class EnemyBuilding;

struct AreaDamageContext {
    Player& player;
    GameField& field;
    std::vector<Enemy>& enemies;
    std::vector<EnemyBuilding>& buildings;
    int targetX;
    int targetY;
};

#endif