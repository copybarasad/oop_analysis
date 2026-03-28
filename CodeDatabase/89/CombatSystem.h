#ifndef COMBAT_SYSTEM_H
#define COMBAT_SYSTEM_H

#include "Player.h"
#include "Enemy.h"
#include "GameGrid.h"
#include <vector>
#include <memory>

enum class Direction;

class CombatSystem {
public:
    CombatSystem(GameGrid& grid);
    
    void playerAttack(Player& player, Direction direction, std::vector<std::unique_ptr<Enemy>>& enemies);

private:
    GameGrid& grid;
    
    bool hasLineOfSight(int fromX, int fromY, int toX, int toY) const;
    Enemy* findEnemyInDirection(Player& player, Direction direction, int& targetX, int& targetY, std::vector<std::unique_ptr<Enemy>>& enemies);
};

#endif