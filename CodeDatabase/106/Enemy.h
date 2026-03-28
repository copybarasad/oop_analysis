#ifndef ENEMY_H
#define ENEMY_H

#include "../entity/Entity.h"

class Enemy : public Entity {
private:
    int damage;

public:
    Enemy(const std::string& enemyName, int startX = 0, int startY = 0, int health = 20);

    void setDamage(int d);
    int getDamage() const { return damage; };
};


#endif