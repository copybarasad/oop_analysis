#pragma once

enum class CellType { Empty, Player, Enemy, Wall };

class Cell {
private:
    CellType type;

public:
    Cell();
    Cell(const Cell& other) = default;
    Cell& operator=(const Cell& other) = default;
    Cell(Cell&& other) noexcept = default;
    Cell& operator=(Cell&& other) noexcept = default;
    
    void setType(CellType t);
    CellType getType() const;
    bool isEmpty() const;
    bool isWall() const;
    bool isOccupied() const;
};