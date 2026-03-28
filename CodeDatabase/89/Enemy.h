#ifndef ENEMY_H
#define ENEMY_H

#include <utility>

class Enemy {
public:
    Enemy(int initialHealth, int initialDamage, int x, int y);

    int getHealth() const;
    int getDamage() const; 
    void takeDamage(int amount);
    bool isAlive() const;

    std::pair<int, int> getPosition() const;
    void setPosition(int x, int y);

private:
    int health;
    int damage;
    std::pair<int, int> position;
};

#endif