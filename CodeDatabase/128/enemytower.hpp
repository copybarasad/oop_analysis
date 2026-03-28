#pragma once
#include <iostream>
#include <cmath>
#include "enemy_building.hpp"   
#include "player.hpp"
#include "constants.hpp"

class EnemyTower : public EnemyStructure {
public:
    EnemyTower(int x, int y, int health, int damage, int radius, int delay);
    
    int getRadius() const noexcept;
    int getDelay() const noexcept;
    int getCounter() const noexcept;
    void SetCounter(int c) noexcept { counter_ = c; }
    void Tick(Game& game) override;

private:
    void ApplyWeakenedDirectDamage(Game& game);
    const int attackRadius_;
};
