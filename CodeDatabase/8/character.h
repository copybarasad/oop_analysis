#pragma once
#include <string>
#include "field.h"

class Character {
protected:
    std::string name;
    int lives;
    int damage;
    int x, y;
    Field& field;

public:
    Character(std::string name, Field& field, int startX, int startY);
    virtual ~Character() = default;

    void setPosition(int newX, int newY);
    void takeDamage(int val);
    int getDamage() const;
    void addLives(int val);
    void removeLives(int val);
    int getLives() const;
    bool isAlive() const;
    std::string getname() const;
    int getX() const;
    int getY() const;
    void setLives(int newLives) { lives = newLives; }
    void setDamage(int newDamage) { damage = newDamage; }
};