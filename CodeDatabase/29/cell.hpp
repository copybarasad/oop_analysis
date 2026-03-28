#ifndef CELL_HPP
#define CELL_HPP

#include <stdexcept>

enum class CellType { kFloor, kWall };

class Cell {
public:
    Cell() noexcept;
    explicit Cell(CellType type) noexcept;

    bool IsPassable() const noexcept;
    CellType Type() const noexcept;
    void SetType(CellType t) noexcept;

private:
    CellType type_;
};

#endif