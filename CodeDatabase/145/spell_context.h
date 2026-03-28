#ifndef SPELL_CONTEXT_H
#define SPELL_CONTEXT_H

#include <vector>
#include <memory>

class Player;
class GameField;
class Enemy;
class EnemyBuilding;
class EnemyTower;
class Ally;

struct SpellContext {
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