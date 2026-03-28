#pragma once
#include <vector>
#include <utility>
#include "Entity.h"
#include "struct/dataTower.h"

class Tower: public Entity {
private:
    int damageCooldown;
    int damageInterval;
    int range = 5;
public:
    Tower(int interval, int dmg) : Entity(200, dmg), damageInterval(interval), damageCooldown(0){}
    Tower(dataTower data);
    ~Tower() override;
    std::vector<std::pair<int,int>> Update(std::vector<std::pair<int,int>> allyPos, std::pair<int,int> towerPos);
    int GetDamageInterval() const;
    //int GetDamageCooldown() const;

    dataTower Save();

};
