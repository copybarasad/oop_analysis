#ifndef GAME_FIELD_H
#define GAME_FIELD_H

#include "Cell.h"
#include "Position.h"
#include "Constants.h"
#include "Unit.h"
#include <vector>
#include <memory>
#include <random>

class Player;
class Enemy;
class EnemyBuilding;
class AllyUnit;

class GameField {
private:
    int fieldSize;
    std::vector<std::vector<Cell>> grid;

    void initializeField();
    void placeObstacles(int obstacleCount, std::uniform_int_distribution<int>& distribution, std::mt19937& generator);
    void placeSlowTerrain(int slowCount, std::uniform_int_distribution<int>& distribution, std::mt19937& generator);

public:
    GameField(int size = DEFAULT_FIELD_SIZE);
    
    GameField(const GameField& other);
    GameField(GameField&& other);
    GameField& operator=(const GameField& other);
    GameField& operator=(GameField&& other);
    ~GameField() = default;

    int getSize() const;
    bool isValidPosition(int x, int y) const;
    bool isValidPosition(const Position& position) const;
    CellType getCellType(int x, int y) const;
    bool isObstacleAt(int x, int y) const;
    bool isSlowAt(int x, int y) const;
    bool isEmptyAt(int x, int y) const;

    void setCellType(int x, int y, CellType type);
    void clearUnitPositions();

    // Обновление состояния поля
    void updateField(const Player& player,
                    const std::vector<std::shared_ptr<Enemy>>& enemies,
                    const std::vector<std::shared_ptr<EnemyBuilding>>& buildings,
                    const std::vector<std::shared_ptr<AllyUnit>>& allies); 

    // Взаимодействие с ловушками
    bool triggerTrapAt(int x, int y, Unit& unit); 
    
    // Дополнительные методы для визуализации
    const Cell& getCell(int x, int y) const;
    const std::vector<std::vector<Cell>>& getGrid() const { return grid; }
};

#endif