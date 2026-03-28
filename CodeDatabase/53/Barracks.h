#ifndef BARRACKS_H
#define BARRACKS_H

#include "Position.h"
#include "Building.h"
#include <string>

class GameField;

class Barracks : public Building {
private:
    int spawnCooldown;   
    int currentCooldown;

public:
    Barracks(int x, int y, int cooldown);
    
    void update(GameField& field);
    
    // Геттеры
    int getCooldown() const;
    int getCurrentCooldown() const;
    
    // Сеттеры
    void setCooldown(int cooldown);
    void setCurrentCooldown(int cooldown);
};

#endif