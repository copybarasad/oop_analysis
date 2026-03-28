#ifndef ENEMY_H
#define ENEMY_H

#include "01_position.h"

class Enemy {
public:
    Enemy(int health, int damage, Position position);

    void takeDamage(int damage);
    void move(Position newPosition);

    int getHealth() const;
    Position getPosition() const;
    int getDamage() const;
    bool isAlive() const;

private:
    int health_;
    int damage_;
    Position position_;
};

#endif