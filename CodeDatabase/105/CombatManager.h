#pragma once

#include "Player.h"
#include "Enemy.h"
#include "Cell.h"
#include "GameVisualize.h"

#include <vector>
#include <utility>
#include <random>

class CombatManager {
    
private:
    std::vector<Enemy>& enemies;
    std::vector<std::pair<int, int>>& enemyPositions;
    std::vector<std::vector<Cell>>& grid;
    int width;
    int height;
    ConsoleGameVisualizer visualizer;

public:
    CombatManager(std::vector<Enemy>& enemiesRef, 
                  std::vector<std::pair<int, int>>& positionsRef,
                  std::vector<std::vector<Cell>>& gridRef,
                  int w, int h);

    // Атака врага игроком
    bool attackEnemy(int x, int y, Player& player);
    
    // Атака игрока врагом
    void attackPlayer(int enemyIndex, Player& player);
    
    // Перемещение врагов
    void moveEnemies(Player& player);
    
    // Удаление мертвых врагов
    void removeDeadEnemies();

private:
    bool isValidPosition(int x, int y) const;
    bool isCellPassable(int x, int y) const;
    bool hasPlayer(int x, int y) const;
    bool hasEnemy(int x, int y) const;
    bool hasBuilding(int x, int y) const;
};