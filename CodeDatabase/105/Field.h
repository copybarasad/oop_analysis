#pragma once

#include "Cell.h"
#include "Player.h"
#include "Enemy.h"
#include "EnemyBuilding.h"
#include "CombatManager.h"
#include "GameVisualize.h"

#include <vector>

class GameField {
private:
    int width, height;
    int playerX, playerY;
    std::vector<std::vector<Cell>> grid;
    std::vector<std::pair<int, int>> enemyPositions;
    std::vector<Enemy> enemies;
    std::vector<std::pair<int, int>> buildingPositions;
    std::vector<EnemyBuilding> buildings;
    CombatManager combatManager;  
    ConsoleGameVisualizer visualizer;

public:
    // Конструкторы
    GameField(int w, int h);
    GameField(const GameField& other);
    GameField(GameField&& other) noexcept;
    GameField& operator=(const GameField& other);
    GameField& operator=(GameField&& other) noexcept;

    // Методы для работы с игроком
    bool setPlayerPosition(int x, int y, Player& player);
    std::pair<int, int> getPlayerPosition() const;

    // Методы для проверки клеток
    bool isCellPassable(int x, int y) const;
    bool hasPlayer(int x, int y) const;
    bool hasEnemy(int x, int y) const;
    bool hasBuilding(int x, int y) const;
    CellType getCellType(int x, int y) const;

    // Методы для работы с врагами
    void moveEnemies(Player& player) { combatManager.moveEnemies(player); }
    void removeDeadEnemies() { combatManager.removeDeadEnemies(); }
    void spawnEnemiesFromBuildings();

    // Геттеры
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    size_t getEnemyCount() const { return enemies.size(); }
    
    // Const-версии методов для доступа к данным
    std::vector<Enemy>& getEnemies() { return enemies; }
    const std::vector<Enemy>& getEnemies() const { return enemies; }
    
    const std::vector<std::pair<int, int>>& getEnemyPositions() const { return enemyPositions; }
    const std::vector<std::pair<int, int>>& getBuildingPositions() const { return buildingPositions; }
    const std::vector<EnemyBuilding>& getBuildings() const { return buildings; }

    // Для сериализации
    void clearAll();
    void setCellType(int x, int y, CellType type);
    void placeEnemy(int x, int y, const Enemy& enemy);
    void placeBuilding(int x, int y, const EnemyBuilding& building);
    void setInternalPlayerPosition(int x, int y);
    
    // Доступ к данным для CombatManager и SaveLoadManager (не-const версии)
    std::vector<std::vector<Cell>>& getGrid() { return grid; }
    std::vector<std::pair<int, int>>& getEnemyPositionsRef() { return enemyPositions; }
    std::vector<std::pair<int, int>>& getBuildingPositionsRef() { return buildingPositions; }
    std::vector<EnemyBuilding>& getBuildingsRef() { return buildings; }

    void addRandomEnemies(int count);
    void addRandomBuildings(int count);
    

private:
    void initializeGrid();
    
    bool attackEnemy(int x, int y, Player& player) { 
        return combatManager.attackEnemy(x, y, player); 
    }
};