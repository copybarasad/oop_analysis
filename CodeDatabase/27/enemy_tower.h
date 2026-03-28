#pragma once
#include "unit.h"
#include "direct_attack.h"

class Player;

class EnemyTower : public Unit {
    DirectAttack derezzPulse;
    bool attacked = false;
protected:
    void on_move_to_point(Point& point) override;
public:
    EnemyTower(Grid& grid, int x, int y, int radius, int damage);
    bool try_attack(Player& player);
};