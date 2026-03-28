#ifndef MOVEMENT_SYSTEM_H
#define MOVEMENT_SYSTEM_H

#include "GameGrid.h"
#include "Player.h"
#include "Enemy.h"
#include "Command.h"  
#include <vector>
#include <memory>

class MovementSystem {
public:
    MovementSystem(GameGrid& grid);
    
    bool movePlayer(Player& player, Direction direction, 
                    const std::vector<std::unique_ptr<Enemy>>& enemies);

    void moveEnemies(std::vector<std::unique_ptr<Enemy>>& enemies, Player& player);

private:
    GameGrid& grid;
    
    bool isValidMove(int x, int y) const;
    bool isPositionOccupiedByEnemy(int x, int y, const std::vector<std::unique_ptr<Enemy>>& enemies) const;
    void updatePosition(int oldX, int oldY, int newX, int newY, CellType type);
};

#endif