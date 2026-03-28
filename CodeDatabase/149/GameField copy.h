#ifndef GAMEFIELD_H
#define GAMEFIELD_H

#include <cstddef>
#include <vector>
#include <memory>
#include "Cell.h"
#include "Position.h"
#include "Enemy.h"
#include <fstream>

class Player;
class SaveManager;

class GameField {
private:
    int playerDamage;
    std::vector<std::vector<Cell>> grid;
    int width;
    int height;
    
    Position playerPosition;
    std::vector<Position> enemyPositions;
    std::vector<std::unique_ptr<Enemy>> enemies;
    std::vector<Position> trapPositions;
    std::vector<int> trapDamages;
    bool towersCanAttackFlag;

    void initializeField();
    Cell& getCell(const Position& pos);
    const Cell& getCell(const Position& pos) const;
    void copyFrom(const GameField& other);
    void moveFrom(GameField&& other);

    friend class SaveManager;

public:
    GameField(int fieldWidth, int fieldHeight);
    GameField(const GameField& other);
    GameField(GameField&& other);
    GameField& operator=(const GameField& other);
    GameField& operator=(GameField&& other);
    ~GameField() = default;
    
    void setPlayerDamage(int damage);
    bool movePlayer(int deltaX, int deltaY);
    void moveEnemies(Player& player);
    Position getPlayerPosition() const;
    bool setPlayerPosition(const Position& pos);
    
    bool addEnemy(std::unique_ptr<Enemy> enemy, const Position& position);
    bool addEnemyFromSave(std::unique_ptr<Enemy> enemy, const Position& pos);
    int getAliveEnemyCount() const;
    bool hasEnemyAtPosition(const Position& pos) const;
    std::vector<Position> getEnemyPositions() const;
    std::vector<int> getEnemyHealths() const;
    std::vector<int> getEnemyDamages() const;
    void damageEnemy(int index, int damage);
    bool isEnemyAlive(int index) const;
    bool isEnemyBuildingAt(const Position& pos) const;
    bool isEnemyTower(int index) const;
    int getEnemyAttackRange(int index) const;
    void damageBuilding(const Position& pos, int damage);
    
    bool placeTrap(const Position& position, int damage);
    void checkTraps();
    const std::vector<Position>& getTrapPositions() const { return trapPositions; }
    const std::vector<int>& getTrapDamages() const { return trapDamages; }
    void clearTraps();
    void resetTowerAttackStates();
    bool canTowerAttack(int towerIndex) const;
    
    bool isValidPosition(const Position& pos) const;
    bool isCellPassable(const Position& pos) const;
    void setCellType(const Position& pos, CellType type);
    CellType getCellType(const Position& pos) const;
    
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    
    const std::vector<std::vector<Cell>>& getGrid() const { return grid; }
    const std::vector<std::unique_ptr<Enemy>>& getEnemies() const { return enemies; }
    
    void setGridCell(int x, int y, CellType type);
    void clearEnemies();
    void addTrapFromSave(const Position& pos, int damage);
    
    void saveFieldState(std::ofstream& file) const;
    void loadFieldState(std::ifstream& file);
};

#endif