#pragma once
#include "entity.hpp"
#include "enemystructure.hpp"

class EnemyBuilding : public EnemyStructure {
public:
    EnemyBuilding(int x, int y, int delay);
    void Tick(Game& game) override;
};