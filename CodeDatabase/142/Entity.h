#pragma once
#include <memory>

class Field;

enum class CombatMode {
    Melee,
    Ranged
};

class Entity {
protected:
    int hp;
    int maxHP;
    int x, y;
    bool alive;

public:
    Entity(int startX, int startY, int health);
    virtual ~Entity() = default;
    
    Entity(const Entity& other) = default;
    Entity& operator=(const Entity& other) = default;
    Entity(Entity&& other) noexcept = default;
    Entity& operator=(Entity&& other) noexcept = default;

    virtual void takeDamage(int damage);
    virtual void setHP(int health);
    virtual bool isAlive() const;
    virtual int getDamage() const = 0;
    
    int getX() const;
    int getY() const;
    int getHP() const;
    int getMaxHP() const;
    
    void setPosition(int newX, int newY);
    virtual char getDisplayChar() const = 0;
};
