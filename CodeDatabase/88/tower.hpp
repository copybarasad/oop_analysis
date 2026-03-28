#ifndef TOWER_H
#define TOWER_H

#include "character.hpp"
#include "enemy.hpp"
#include <memory>

class Tower : public Character {
private:
    int totalCooldown;
    int currentCooldown;
    char icon = 'T';
    
public:
    Tower(int startHealth, int startDamage, int startX, int startY, int cooldown);
    
    char getIcon()  override { return icon; }
    
    bool isTimerUp();
    void resetCooldown() { currentCooldown = totalCooldown; }
    int getCurrentCooldown() const { return currentCooldown; }
    int getTotalCooldown() const { return totalCooldown; }
    
    
};

#endif