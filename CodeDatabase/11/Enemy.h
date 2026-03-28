#ifndef ENEMY_H
#define ENEMY_H

#include <vector>
#include "Trap.h"

class Field;
class Player;
class EventLogger;

class Enemy {
public:
    Enemy(int startX, int startY, int health, int damage);
    void processTurn(Player& player, Field& field, std::vector<Trap>& traps, EventLogger* logger);
    
    bool isAlive() const;
    void takeDamage(int amount);
    int getX() const;
    int getY() const;
    int getHealth() const;
    int getDamage() const;

private:
    void move(int newX, int newY, Field& field);
    int x, y, health, damage;
};
#endif