#pragma once
#include "Character.h"

class Enemy : public Character {
private:
    int damage;
    
public:
    Enemy(int x = 0, int y = 0,int Health = 30, int Damage = 8);
    ~Enemy() = default;
    int getDamage() const;
};