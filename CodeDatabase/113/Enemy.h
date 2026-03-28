#ifndef ENEMY_H
#define ENEMY_H

#include "BaseEnemy.h"

class Enemy : public BaseEnemy {
public:
    Enemy(int health, int damage, int x, int y);

    void makeMove(GameField& field, Player& player) override;
};

#endif // ENEMY_H