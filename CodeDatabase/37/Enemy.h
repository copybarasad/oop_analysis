#ifndef ENEMY_H
#define ENEMY_H

#include "Position.h"

class GameField;  

class Enemy {
private:
    int health;
    int damage;
    Position position;

public:
    Enemy(int health, int damage, const Position& startPos);
    
    int getHealth() const { return health; }
    int getDamage() const { return damage; }
    Position getPosition() const { return position; }
    bool isAlive() const { return health > 0; }
    
    void moveTowards(const Position& targetPos, const GameField& field);
    void attack(class Player& player) const;
    void takeDamage(int amount) { health -= amount; }
    void setPosition(const Position& pos) { position = pos; }
};

#endif