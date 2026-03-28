#ifndef CHARACTER_H
#define CHARACTER_H

#include <string>
#include "Position.h"

class Character {
protected:
    std::string name;
    int health;
    int max_health;
    int damage;
    Position position;
    bool alive;
    int freeze_time;
    bool inSaloon;

public:
    Character(std::string characterName, int startHealth, int startDamage, int startX, int startY);
    
    virtual void take_damage(int amount);
    bool isAlive() const;
    void heal(int amount);
    
    // Геттеры
    std::string getName() const;
    int getHealth() const;
    int getMaxHealth() const;
    int getDamage() const;
    int getFreeze_time() const;
    int getX() const;
    int getY() const;
    bool getInsaloon() const;
    
    // Сеттеры
    void setPosition(int newX, int newY);
    void setHealth(int newHealth);
    void setDamage(int newDamage);
    void setFreeze_time(int newFreze_time);
    void setinSaloon(bool new_inSaloon);
    void setMaxHealth(int newMaxHealth);
    void setAlive(bool isAlive);
    void setName(const std::string& newName);
    
};

#endif