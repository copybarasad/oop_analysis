#pragma once

#include "EnemyBuilding.h"
#include <vector>

class EnemyTower : public EnemyBuilding 
{
protected:
    int HP;
    int maxHP;
    int range;
    int attackCD;
    int currentCD;
public:
    EnemyTower(int X_coord = 0, int Y_coord = 0, int range = 1);
    int getHP() const;
    void take_damage(int damage);
    bool isAlive() const;
    std::vector<Coordinates> getAttackCoordinates() const;
    bool updateAndAttack();
    std::string serialize() const override;
    void deserialize(const std::string& data) override;
};