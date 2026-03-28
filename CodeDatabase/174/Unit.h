#ifndef UNIT_H
#define UNIT_H

#include "Position.h"
#include "CellType.h"

class Unit {
protected:
    int maxHealthPoints;  // Добавляем максимальное здоровье
    int healthPoints;
    int damage;
    Position position;
     
public:
    Unit();
    Unit(int health, int damageValue, int x, int y);
    virtual ~Unit() = default;

    int getHealthPoints() const;
    int getMaxHealthPoints() const;  // Новый метод
    int getDamage() const;
    Position getPosition() const;
    
    void setPosition(int x, int y);
    void setHealth(int health);
    void setMaxHealth(int maxHealth);  // Новый метод
    void setDamage(int damage);
    void takeDamage(int damageAmount);
    bool isAlive() const;
    
    virtual CellType getCellType() const = 0;
};

#endif