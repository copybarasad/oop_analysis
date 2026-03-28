#pragma once
#include "types.hpp"
#include "GameObjects/Entities/Tower.hpp"
#include "GameObjects/Entities/interfaces.hpp"

class AttackTower : Fightable, public Tower {
public:
    AttackTower();

    Coordinates getPosition() override;
    void setPosition(Coordinates pos);

    int getHealthPoints();
    void setHealthPoints(int hp);
    void minusHealthPoints(int hp) override;

    int getMaxHP();
    void setMaxHP(int hp);

    
private:
    Coordinates position;
    int healthPoints;
    int maxHP;
};
