#ifndef CHARACTER_H
#define CHARACTER_H

#include <stdexcept>

class Character {
protected:
    int health;
    int positionX;
    int positionY;

    void validateHealth(int healthValue) const {
        if (healthValue < 0) {
            throw std::invalid_argument("Health cannot be negative");
        }
    }

    void validatePosition(int x, int y, int maxX, int maxY) const {
        if (x < 0 || y < 0 || x >= maxX || y >= maxY) {
            throw std::invalid_argument("Position is out of bounds");
        }
    }

public:
    Character(int startX, int startY, int initialHealth, int maxX, int maxY);

    virtual ~Character() = default;

    int getHealth() const;

    void getPosition(int &x, int &y) const;

    void takeDamage(int damage);

    void setPosition(int x, int y);

    bool isAlive() const;

    virtual int getDamage() const = 0;
};

#endif
