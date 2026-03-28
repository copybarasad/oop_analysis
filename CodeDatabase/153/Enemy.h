#ifndef ENEMY_H
#define ENEMY_H

#include "Entity.h"

class Enemy : public Entity {
private:
    int x, y;

public:
    Enemy(int startX, int startY);
    int getX() const;
    int getY() const;
    void move(int newX, int newY);

    void setHealth(int newHealth);
};

#endif

