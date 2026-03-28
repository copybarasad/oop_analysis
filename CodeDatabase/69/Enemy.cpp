#include "Enemy.h"

Enemy::Enemy(std::string id, Position pos, int hp, int reward, int damage)
    : Entity(std::move(id), pos, hp)
    , reward_(reward)
    , damage_(damage)
{}

int Enemy::GetReward() const 
{ 
    return reward_; 
}

int Enemy::GetDamage() const 
{ 
    return damage_; 
}