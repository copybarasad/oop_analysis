#pragma once

#include "Humanoid.h"

class Enemy:public Humanoid 
{
public:
    Enemy(bool IsPlayer = false, bool IsAlive = true, ATKtype type = ATKtype::melee, std::string Body = "Ѫ ", 
          int HP = 3, int ATK = 1, int X_coord = 0, int Y_coord = 0, int Speed = 1);
    
    Coordinates moveto(Direction where);
    void move(Coordinates destination, int damage, int Speed);
    std::string serialize() const;
    void deserialize(const std::string& data);
};