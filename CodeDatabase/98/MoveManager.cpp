#include "MoveManager.h"
#include <cstdlib>

Move::MoveResult Move::MovePlayerByDirection(Player& player, int& playerX, int& playerY, 
                                           const Field& field, char direction) {
    MoveResult result;
    result.success = false;
    result.newX = playerX;
    result.newY = playerY;
    
    if (direction != 'w' && direction != 's' && direction != 'a' && direction != 'd') {
        result.message = "Неверное направление";
        return result;
    }
    
    int newX = playerX;
    int newY = playerY;
    int speed = player.GetSpeed();
    
    for(int i = 0; i < speed; i++) {
        if(direction == 'w') newY--;
        else if (direction == 's') newY++;
        else if (direction == 'a') newX--;
        else if (direction == 'd') newX++;
        
        if (newX < 0 || newY < 0 || newX >= field.GetWidth() || newY >= field.GetHeight()) {
            result.message = "Вы не можете покинуть границы поля";
            return result;
        }
        
        if(field.GetCell(newX, newY).IsBlocked()) {
            result.message = "Эту клетку нельзя пройти";
            return result;
        }
        
        result.newX = newX;
        result.newY = newY;
    }
    
    result.success = true;
    result.message = "Успешное перемещение";
    return result;
}


void Move::MoveEnemy(std::vector<Enemy>& enemies, 
                    std::vector<std::pair<int, int>>& enemyPositions, 
                    const Field& field, int playerX, int playerY) {
    for (size_t i = 0; i < enemies.size(); i++) {
        int dir = std::rand() % 4;
        int newX = enemyPositions[i].first;
        int newY = enemyPositions[i].second;
        
        if (dir == 0) newY--;
        else if (dir == 1) newY++;
        else if (dir == 2) newX--;
        else if (dir == 3) newX++;
        
        if (newX < 0 || newY < 0 || newX >= field.GetWidth() || newY >= field.GetHeight()) 
            continue;
        if(field.GetCell(newX, newY).IsBlocked()) 
            continue;
        if (newX == playerX && newY == playerY) 
            continue;
        
        bool isbusyanyenemy = false;
        for (size_t j = 0; j < enemies.size(); j++) {
            if(i != j && enemyPositions[j].first == newX && enemyPositions[j].second == newY) {
                isbusyanyenemy = true;
                break;
            }
        }
        
        if (isbusyanyenemy) continue;
        
        enemyPositions[i].first = newX;
        enemyPositions[i].second = newY;
    }
}