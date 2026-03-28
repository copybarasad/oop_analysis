#ifndef ENEMY_AI_SYSTEM_H
#define ENEMY_AI_SYSTEM_H

#include "Enemy.h"
#include "Player.h"
#include "GameField.h"
#include "EnemyTower.h"
#include "MovementSystem.h"
#include <vector>

class EnemyAISystem {
private:
    GameField& field;
    Player& player;
    MovementSystem& movementSystem;
    
public:
    EnemyAISystem(GameField& gameField, Player& gamePlayer, MovementSystem& moveSystem);
    void updateEnemies(std::vector<Enemy> &enemies);
    void updateTowers(std::vector<EnemyTower> &towers);
    Position calculateEnemyMove(const Enemy &enemy) const;
};

#endif