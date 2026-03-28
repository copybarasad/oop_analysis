#ifndef GAME_ENEMY_H
#define GAME_ENEMY_H

#include "../utils/Point.h"
#include <iostream>

namespace Game {

class Enemy {
public:
    Enemy(Utils::Point initialPosition, int initialHealth = 50, int initialDamage = 5);
    Enemy(std::istream& is); // Конструктор для загрузки

    void move(const Utils::Point& newPosition);
    void takeDamage(int damageAmount);

    int getHealth() const;
    int getDamage() const;
    const Utils::Point& getPosition() const;
    bool isAlive() const;

    void serialize(std::ostream& os) const;
    
private:
    void deserialize(std::istream& is);

    Utils::Point position;
    int health;
    int damage;
};

} 

#endif