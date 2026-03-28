#ifndef SPAWN_SYSTEM_H
#define SPAWN_SYSTEM_H

#include "GameGrid.h"
#include "Player.h"
#include "Enemy.h"
#include <vector>
#include <memory>

class SpawnSystem {
public:
    SpawnSystem(GameGrid& grid);
    
    void placePlayer(std::unique_ptr<Player> player);
    void addEnemy(std::unique_ptr<Enemy> enemy, std::vector<std::unique_ptr<Enemy>>& enemies);

private:
    GameGrid& grid;
    
    bool findEmptyPosition(int& x, int& y) const;
    bool findGoodPosition(int& x, int& y) const;
    bool hasAccessibleNeighbors(int x, int y) const;
    void createGuaranteedPlayerPosition(int& x, int& y);
};

#endif