#pragma once
#include "Character.h"

class Ally : public Character {
private:
    int damage;
    
public:
    Ally(int x = 0, int y = 0,int Health = 50, int Damage = 10);
    ~Ally() = default;
    int getDamage() const;
};