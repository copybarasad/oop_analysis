#ifndef COWBOYTOWER_H
#define COWBOYTOWER_H

#include "Building.h"
#include "Player.h"
#include "Position.h"
#include <string>
#include <iostream>

class GameField;

class CowboyTower : public Building {
private:
    int currentCooldown;
    int attackCooldown;
    int attackRange;
public:
    CowboyTower(int x, int y, int health, int cooldown, int range);
    
    void update(GameField& field, Player& player);
    void attack(GameField& field, Player& player);
    
    // Геттеры
    int getAttackCooldown() const;
    int getAttackRange() const;
    int getCurrentCooldown() const;
    
    // Сеттеры
    void setAttackCooldown(int cooldown);
    void setAttackRange(int range);
    void setCurrentCooldown(int cooldown);

};


#endif