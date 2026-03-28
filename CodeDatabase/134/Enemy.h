#pragma once

#include <string>

class Enemy{
public:
    Enemy(int maxHealth, int damage);

    void Move(int dx, int dy);
    void TakeDamage(int damage_amount);
    bool IsAlive() const;

    int GetX() const;
    int GetY() const;
    std::pair<int, int> GetPos() const;
    int GetHealth() const;
    int GetDamage() const;

private:
    int health;
    int maxHealth;
    int attackDamage;
    int posX;
    int posY;
};