#ifndef ENEMY_H
#define ENEMY_H

#include "hero.hpp"
#include "player.hpp"

#include <string>
#include <iostream>

class Enemy : public Hero{
private:
    int x_;
    int y_;
    bool has_attacked_ = false;
public:
    Enemy(const std::string& name, int health, int damage);

    void SetPosition(int x, int y);
    int GetX() const;
    int GetY() const;

    void TakeDamage(int amount) override;
    void Attack(Hero& target) override;

    bool HasAttacked() const;
    void SetAttacked(bool value) { has_attacked_ = value; }
};

#endif