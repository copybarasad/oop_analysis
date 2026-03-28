#ifndef GAME_FIELD_H
#define GAME_FIELD_H

#include "cell.h"
#include <vector>
#include <utility>
#include <cstddef>

class Enemy;
class EnemyBuilding;
class EnemyTower;
class Player;
class Ally;

struct Trap {
    int x, y;
    int damage;
};

class GameField {
private:
    size_t width;
    size_t height;
    std::vector<std::vector<Cell>> grid;
    std::vector<Trap> traps;
    
public:
    GameField(size_t width, size_t height);
    GameField(const GameField& other);
    GameField(GameField&& other);
    GameField& operator=(const GameField& other);
    GameField& operator=(GameField&& other);
    
    size_t getWidth() const;
    size_t getHeight() const;
    
    bool isValidPosition(int x, int y) const;
    bool isPassable(int x, int y) const;
    bool isSlowing(int x, int y) const;
    bool isEmpty(int x, int y) const;
    
    void setCell(int x, int y, CellType type);
    
    void generateImpassableCells(int count);
    void generateSlowingCells(int count);
    
    
    void placeTrap(int x, int y, int damage);
    const std::vector<Trap>& getTraps() const;
    void removeTrap(int x, int y);
    bool hasTrapAt(int x, int y) const;
    int getTrapDamageAt(int x, int y) const;

    
    bool isOccupied(int x, int y,
                    const std::vector<Enemy>& enemies,
                    const std::vector<EnemyBuilding>& buildings,
                    const std::vector<EnemyTower>& towers,
                    const Player* player = nullptr,
                    const std::vector<Ally>* allies = nullptr) const;
    
    bool isStructureAt(int x, int y,
                       const std::vector<EnemyBuilding>& buildings,
                       const std::vector<EnemyTower>& towers) const;
    
private:
    void deepCopy(const GameField& other);
};

#endif