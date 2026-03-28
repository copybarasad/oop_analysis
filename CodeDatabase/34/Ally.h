#ifndef GAME_ALLY_H
#define GAME_ALLY_H

#include "../utils/Point.h"
#include <iostream>

namespace Game {

class Ally {
public:
    Ally(Utils::Point initialPosition, int initialHealth = 40, int initialDamage = 8);
    Ally(std::istream& is); // Конструктор для загрузки

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