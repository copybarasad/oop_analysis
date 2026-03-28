#pragma once
#include <utility>

class Tower {
private:
    int health;
    int radius;
    std::pair<int, int> position;
    int damage;
    int cooldown;
    int attackCooldown;

public:
    Tower(int health, int radius, std::pair<int, int> position, int damage);
    
    std::pair<int, int> getPosition() const;
    bool isAlive() const;
    void takeDamage(int dmg);
    bool isInRange(std::pair<int, int> target) const;
    bool canAttack() const;
    void setCooldown();
    void updateCooldown();
    int getHealth() const;
    int getRadius() const;
    int getDamage() const;
};