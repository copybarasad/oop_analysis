#ifndef ENEMY_TOWER_H
#define ENEMY_TOWER_H

#include "Entity.h"

class GameField;
class Player;

class EnemyTower : public Entity {
private:
    int range;
    int damage;
    int cooldownRemaining;

public:
    EnemyTower(int x, int y, int hp, int range, int damage, int cooldown);
    int getX() const { return Entity::getX(); }
    int getY() const { return Entity::getY(); }
    void tickAndAttack(GameField& field, Player& player);
};

#endif 

