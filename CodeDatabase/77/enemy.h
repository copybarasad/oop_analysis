#ifndef ENEMY_H
#define ENEMY_H

#include "entity.h"
#include "player.h"

class Enemy : public Entity {
public:
    Enemy() : Entity(50, 0, 0, "Enemy", 'E', EVIL, 5, 10) {}
    Enemy(int posX, int posY, int enemyBaseHp) : Entity(enemyBaseHp, posX, posY, "Enemy", 'E', EVIL, 5, 10) {}
    std::vector<int> calculate_new_move(const Player& player, GameField& field);
};

#endif