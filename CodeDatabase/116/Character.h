#pragma once
#include "../nlohmann/json.hpp"

class Character {
private:
    int health;
    int x,y;
    
public:
    Character(int X = 0, int Y = 0,int Health = 10);
    virtual ~Character() = default;
    int getHealth() const;
    bool isAlive() const;
    void takeDamage(int dam_points);
    int getX() const;
    int getY() const;
    void setPosition(int newX, int newY);
    void setHealth(int newHealth){
        health = newHealth;
    }
};