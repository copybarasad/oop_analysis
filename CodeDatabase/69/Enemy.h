#pragma once
#include "Entity.h"
#include <string>

class Enemy : public Entity 
{
public:
    Enemy(std::string id, Position pos, int hp, int reward, int damage);
    int GetReward() const;
    int GetDamage() const; 

private:
    int reward_;
    int damage_;
};