#ifndef ENEMY_H
#define ENEMY_H

#include "game_entity.h"
#include <iostream>

class Enemy {
private:
    Position position;
    int health;
    int damage;

public:
    Enemy(int startX = 0, int startY = 0, int health = 30, int damage = 10);

    Position getPosition() const;
    int getHealth() const;
    int getDamage() const;

    void move(int deltaX, int deltaY);
    void takeDamage(int damage);
    bool isAlive() const;

    void save(std::ostream& os) const;
    void load(std::istream& is);
};

#endif 