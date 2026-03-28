#ifndef ENTITY_H
#define ENTITY_H

#include <string>

class Entity {
private:
    int health;
    int maxHealth;
    int x, y;
public:
    Entity(int health, int maxHealth) : health(health), maxHealth(maxHealth), x(0), y(0) {}

    int getX() const { return x; }
    int getY() const { return y; }
    void setPosition(int newX, int newY);

    int getHealth() const { return health; }
    int getMaxHealth() const { return maxHealth; }
    void setHealth(int health) { this->health = health; }
    void setMaxHealth(int maxHealth) { this->maxHealth = maxHealth; }
    void takeDamage(int damage);
    virtual int getDamage() const = 0;
    virtual std::string getEntityName() const = 0;
    bool isAlive() const;
};

#endif
