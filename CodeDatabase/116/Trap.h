#pragma once
#include "Character.h"

class Trap : public Character {
private:
    int damage;
public:
    Trap(int x = 0, int y = 0,int Health = 1, int Damage = 25);
    ~Trap() = default;
    int getDamage() const;
};