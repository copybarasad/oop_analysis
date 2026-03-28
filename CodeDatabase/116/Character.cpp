#include "Character.h"
#include <iostream>

Character::Character(int X,int Y,int Health) 
    : health(Health), x(X),y(Y) {}

int Character::getHealth() const {
    return health;
}

bool Character::isAlive() const {
    return health > 0;
}

void Character::takeDamage(int dam_points) {
    health -= dam_points;
    if (health < 0) { 
        health = 0; 
    }
}

int Character::getX() const { return x; }

int Character::getY() const { return y; }

void Character::setPosition(int newX, int newY) { x = newX; y = newY; }
    