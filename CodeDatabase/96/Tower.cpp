#include <vector>
#include <utility>
#include "Tower.h"
#include <iostream>

Tower::~Tower() = default;

Tower::Tower(dataTower data): Entity(data.health, data.damage), damageInterval(data.damageInterval), damageCooldown(data.damageCooldown), range(data.range){
    SetStunDuration(data.stunDuration);
}


std::vector<std::pair<int,int>> Tower::Update(std::vector<std::pair<int,int>> allyPos, std::pair<int,int> towerPos) {
    std::vector<std::pair<int,int>> result;
    if(damageCooldown > 0) {
        damageCooldown--;
        return result;
    }
    for(const auto& pos : allyPos) {
        int dx = pos.first - towerPos.first;
        dx = dx*dx;
        int dy = pos.second - towerPos.second;
        dy = dy*dy;
        int r2 = range*range;
        if(dx+dy <= r2) {
            result.push_back(pos);
            damageCooldown = damageInterval;
            return result;
        }
    }
    return result;
}

int Tower::GetDamageInterval() const {
    return damageInterval;
}

dataTower Tower::Save(){
    dataTower data;
    data.damageCooldown=damageCooldown;
    data.damageInterval=damageInterval;
    data.range=range;
    data.damage=damage;
    data.health=health;
    data.stunDuration=stunDuration;
    return data;
}