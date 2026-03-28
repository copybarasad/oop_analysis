#ifndef BUILDING_H
#define BUILDING_H

#include "Position.h"
#include <string>

class GameField;

class Building  {
protected:
    Position position;
    int health;
    bool NotDestroyed;

public:
    Building(int x, int y, int health, bool IsNotDestroyed);
    
    void takeDamage(int damage);
    bool IsNotDestroyed();

    // Геттеры
    int getX() const;
    int getY() const;
    int getHealth() const;
    bool getIsNotDestroyed() const;
    
    // Сеттеры
    void setPosition(int x, int y);
    void setHealth(int newHealth);
    void setIsNotDestroyed(bool status);


};

#endif