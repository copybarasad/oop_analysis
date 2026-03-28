#include "Ally.h"
#include <cmath>
#include <iostream>
#include <cstdlib>
#include <ctime>

Ally::Ally(int x, int y) 
    : pos(x, y), health(30), damage(8), alive(true) {}

void Ally::makeMove(const Position& playerPos, int width, int height) {
    if (!alive) return;
    
    int newX = pos.getX();
    int newY = pos.getY();
    
    // Случайное направление движения
    int direction = rand() % 4;
    switch (direction) {
        case 0: if (newY > 0) newY--; break;           
        case 1: if (newY < height - 1) newY++; break;   
        case 2: if (newX > 0) newX--; break;           
        case 3: if (newX < width - 1) newX++; break; 
    }
    
    setPos(newX, newY);
}

void Ally::takeDamage(int damage) {
    if (damage > 0) {
        health -= damage;
        if (health <= 0) {
            alive = false;
        }
    }
}

const Position& Ally::getPos() const {
    return pos;
}

bool Ally::isAlive() const {
    return alive;
}

int Ally::getHealth() const {
    return health;
}

int Ally::getDamage() const {
    return damage;
}

void Ally::setPos(int x, int y) {
    pos.set(x, y);
}