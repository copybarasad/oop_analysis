#ifndef MOVEMANAGER_H
#define MOVEMANAGER_H

#include <vector>
#include <string>  
#include "field.h"
#include "enemy.h"
#include "player.h"

class Move {
public:
    // для результата перемещения
    struct MoveResult {
        bool success;
        int newX;
        int newY;
        std::string message;  
    };
    
    static MoveResult MovePlayerByDirection(Player& player, int& playerX, int& playerY, 
                                          const Field& field, char direction);
    
    
    static void MoveEnemy(std::vector<Enemy>& enemies, 
                         std::vector<std::pair<int, int>>& enemyPositions, 
                         const Field& field, int playerX, int playerY);
};

#endif