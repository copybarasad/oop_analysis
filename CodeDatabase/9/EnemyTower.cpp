#include "EnemyTower.h"

EnemyTower::EnemyTower(std::pair<int,int> pos, int range, int damage, int cooldown, int hp){
    pos_ = pos;
    range_ = range;
    damage_ = damage;
    cooldown_ = cooldown;
    cooldownLeft_ = 0;
    hp_ = hp;
}

std::pair<int,int> EnemyTower::position() const{
    return pos_;
}

int EnemyTower::Chebyshev(std::pair<int,int> a, std::pair<int,int> b) const{
    int dx = a.first - b.first; if(dx < 0) dx = -dx;
    int dy = a.second - b.second; if(dy < 0) dy = -dy;
    return dx > dy ? dx : dy;
}

void EnemyTower::onTurn(Player& player){
    if(!isAlive()){
        return;
    }
    if(cooldownLeft_ > 0){
        cooldownLeft_ -= 1;
        return;
    }
    if(Chebyshev(pos_, player.position()) <= range_){
        player.takeDamage(damage_);
        cooldownLeft_ = cooldown_;
    }
}

int EnemyTower::hp() const{
    return hp_;
}

bool EnemyTower::isAlive() const{
    return hp_ > 0;
}

void EnemyTower::takeDamage(int dmg){
    if(dmg < 0){
        return;
    }
    hp_ -= dmg;
    if(hp_ < 0){
        hp_ = 0;
    }
}

int EnemyTower::damage() const{
    return damage_;
}

int EnemyTower::cooldown() const{
    return cooldown_;
}

void EnemyTower::setHp(int hp){
    hp_ = hp;
    if(hp_ < 0) hp_ = 0;
}

void EnemyTower::setCooldown(int value){
    cooldown_ = value;
    if(cooldown_ < 0) cooldown_ = 0;
}