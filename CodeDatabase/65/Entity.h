#pragma once

#include "GameObject.h"

class Entity : public GameObject {
protected:
    int health;
    int maxHealth;
    int damage;
    int speed;
    int canMove = 0;

public:
    Entity(int x, int y, char sprite, std::string color,
           int health, int damage, int speed = 1, ObjectTag tags = ObjectTag::TAG_NONE)
        : GameObject(x, y, sprite, color, true, tags),
        health(health), maxHealth(health), damage(damage),
        speed(speed) {}

    virtual ~Entity() = default;

    // Getters
    int getHealth() const { return health; }
    int getMaxHealth() const { return maxHealth; }
    int getDamage() const { return damage; }
    int getSpeed() const { return speed; }
    int getCanMove() const { return canMove; }

    // Setters
    void setHealth(int h) { health = h; }
    void setMaxHealth(int h) { maxHealth = h; }
    void setDamage(int d) { damage = d; }
    void setSpeed(int s) { speed = s; }
    void setCanMove(int step) { canMove = step; }

    void takeDamage(int dmg) {
        health -= dmg;
        if (health < 0) health = 0;
    }

    virtual void move(int dx, int dy) {
        x += dx;
        y += dy;
    }

    bool isAlive() const { return health > 0; }
};
