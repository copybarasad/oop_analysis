#pragma once
#include <string>
#include "field.h"
#include "constants.h"

class Character {
protected:
    std::string name;
    int lives;
    int damage;
    int x, y;
    Field& field;

public:
    Character(std::string name, Field& field, int startX, int startY, int initialLives);
    virtual ~Character() = default;
    
    void takeDamage(int val);
    int getDamage() const;
    int getLives() const;
    bool isAlive() const;
    std::string getname() const;
    int getX() const;
    int getY() const;
};
