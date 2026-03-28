#ifndef ENEMY_H
#define ENEMY_H

#include "Character.h"
#include <iostream>

class Player;

class Enemy : public Character {
private:
    int damagePower;

public:
    Enemy(std::string name, Field* field, int startX, int startY, int level);
    Enemy(std::istream& is, Field* newField);
    
    // ИИ врага
    void moveTowards(const Player& player);
    int getDamagePower() const;
    
    // Сохранение
    void save(std::ostream& os) const;
    
    // Геттеры для рендерера (const версии)
    std::string getName() const { return name; }
    int getLives() const { return lives; }
    int getMaxLives() const { return maxLives; }
    int getX() const { return x; }
    int getY() const { return y; }
    bool isAlive() const { return lives > 0; }
    
    // Необходимые методы
    void setField(Field* newField);
};

#endif // ENEMY_H