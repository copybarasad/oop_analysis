#pragma once
#include <stdexcept>
#include <iostream>

class Entity {
public:
    Entity(int health, int damage);
    virtual ~Entity() = default;

    int getHealth() const noexcept {return health_; }   
    int getDamage() const noexcept { return damage_; }
    bool isAlive() const noexcept { return health_ > 0; }
    void setHealth(int h) noexcept { health_ = h; }
    void TakeDamage(int amount);

    virtual int getEffectiveDamage() const noexcept { return getDamage(); }
    virtual void Attack(Entity& target);

protected: 
    int health_;
    int damage_;
};
