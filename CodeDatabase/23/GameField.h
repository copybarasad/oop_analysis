#ifndef GAMEFIELD_H
#define GAMEFIELD_H

#include "Cell.h"
#include "Player.h"
#include "Enemy.h"
#include "EnemyBuilding.h"
#include "EnemyTower.h"
#include <map>
#include <vector>
#include <memory>
#include <random>
#include "Constants.h"

class GameField {
private:
    int width;
    int height;
    Cell** cells;
    Player* player;
    std::map<std::pair<int,int>, Enemy*> enemies;
    std::map<std::pair<int,int>, EnemyBuilding*> buildings;
    std::map<std::pair<int,int>, EnemyTower*> towers;
    std::mt19937 rng;
    std::map<std::pair<int,int>, int> traps; 
    std::vector<std::vector<bool>> reachable;

    void generateTerrain();
    void placeSlowingCells();
    void placeEnemies();
    void placeBuildings();
    void placeTowers();
    void smoothTerrain();
    int countNeighbors(int x, int y, CellType type);
    void findValidPosition(int& x, int& y, bool requireReachable = false);
    void computeReachableFromPlayer();
    bool isReachable(int x, int y) const;

public:
    GameField(int width, int height);
    GameField(const GameField& other);
    GameField(GameField&& other) noexcept;
    ~GameField();
    
    GameField& operator=(const GameField& other);
    GameField& operator=(GameField&& other) noexcept;
    
    int getWidth() const;
    int getHeight() const;
    CellType getCellType(int x, int y) const;
    Player* getPlayer() const;
    const std::map<std::pair<int,int>, Enemy*>& getEnemies() const;
    const std::map<std::pair<int,int>, EnemyBuilding*>& getBuildings() const;
    const std::map<std::pair<int,int>, EnemyTower*>& getTowers() const;
    
    bool movePlayer(int dx, int dy);
    bool attackEnemy(int dx, int dy);
    void moveEnemies();
    void updateBuildings();
    void updateTowers();
    void display() const;
    bool isPositionValid(int x, int y) const;
    
    void removeDeadEnemies();
    void addEnemy(Enemy* enemy);
    void addBuilding(EnemyBuilding* building);
    void addTower(EnemyTower* tower);
    bool hasEnemyAt(int x, int y) const;
    bool hasBuildingAt(int x, int y) const;
    bool hasTowerAt(int x, int y) const;
    bool hasReachablePathTo(int x, int y) const { return isReachable(x, y); }

    void placeTrap(int x, int y, int damage);
    void checkTrapAt(int x, int y);
    bool hasTrapAt(int x, int y) const;
};

#endif 
