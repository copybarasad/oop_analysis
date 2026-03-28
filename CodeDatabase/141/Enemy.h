#ifndef ENEMY_H
#define ENEMY_H

#include "Character.h"

class Enemy : public Character {
private:
    int damage;

public:
    Enemy(int startX, int startY, int enemyHealth, int enemyDamage, int maxX, int maxY);

    int getDamage() const override;
};

#endif
