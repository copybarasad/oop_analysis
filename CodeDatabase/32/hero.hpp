#ifndef HERO_H
#define HERO_H

#include <iostream>
#include <string>
#include <algorithm>

#define MIN_HEALTH 0
#define MIN_DAMAGE 0

class Hero {
protected:
    std::string name_;
    int health_;
    int damage_;

public:
    Hero(const std::string& name, int health, int damage);

    std::string GetName() const;
    int GetHealth() const;
    int GetDamage() const;

    void SetHealth(int health) { health_ = std::max(health, MIN_HEALTH); }
    void SetDamage(int damage) { damage_ = std::max(damage, MIN_DAMAGE); }

    virtual void TakeDamage(int amount);
    virtual void Attack(Hero& other);

    bool IsAlive() const;
};

#endif