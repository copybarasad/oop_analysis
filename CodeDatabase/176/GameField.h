#ifndef GAME_FIELD_H
#define GAME_FIELD_H

#include "GameCell.h"
#include "GameEnums.h"
#include <vector>
#include <memory>
#include <utility>
#include <optional>

class GameField {
private:
    std::vector<std::vector<GameCell>> cells;
    int width;
    int height;
    std::vector<std::pair<int, int>> trapPositions;
    std::vector<std::pair<int, int>> allyPositions;

    void initializeField();

public:
    GameField(int fieldWidth, int fieldHeight);
    GameField(const GameField& other);
    GameField(GameField&& other) noexcept;
    GameField& operator=(const GameField& other);
    GameField& operator=(GameField&& other) noexcept;
    
    int getWidth() const;
    int getHeight() const;
    bool isValidPosition(int x, int y) const;
    const GameCell& getCell(int x, int y) const;
    bool isCellPassable(int x, int y) const;
    std::vector<std::pair<int, int>> getAdjacentPositions(int x, int y) const;
    
    bool hasEnemyAt(int x, int y) const;
    std::optional<Enemy> getEnemyAt(int x, int y) const;
    void removeEnemyAt(int x, int y);
    void addEnemyAt(int x, int y, Enemy enemy);
    void placeTrap(int x, int y);
    bool hasTrapAt(int x, int y) const;
    void triggerTrap(int x, int y);
    void placeAlly(int x, int y);
    bool hasAllyAt(int x, int y) const;
    void removeAlly(int x, int y);
    
    void setCellType(int x, int y, CellType cellType);
    void clearField();
    const std::vector<std::pair<int, int>>& getTrapPositions() const { return trapPositions; }
    const std::vector<std::pair<int, int>>& getAllyPositions() const { return allyPositions; }
    GameCell& getCell(int x, int y);
};

#endif
