#pragma once

#include <iostream>
#include <stdexcept>
#include "entity.h"

class Player : public Entity{
private:
    int maxHealth;
    int armor;
    int points;
    bool isMelee;
    
public:
    Player(int startHealth=100, int startDamage=10, int startArmor=50);

    int getArmor() const;
    int getPoints() const;
    int getMaxHealth() const;
    bool getFightType() const;

    void setMaxHealth(int new_health);
    void setDamage(int dmg);
    void heal(int amount);
    void addPoints(int newPoints);
    void switchFightType();
    
    void takeDamage(int damage) override;
};