#ifndef ENEMYBASE_H
#define ENEMYBASE_H

#include "position.h"
#include "movingenemy.h"
#include <vector>

class GameField;

class EnemyBase {
private:
    Position position;
    int health;
    int spawnCooldown;
    int maxSpawnCooldown;
    
public:
    EnemyBase(Position pos = Position(0,0), int health = 100);
    
    void takeTurn(GameField* field, std::vector<MovingEnemy>& enemies);
    void takeDamage(int amount);
    bool isAlive() const;
    Position getPosition() const;
};

#endif