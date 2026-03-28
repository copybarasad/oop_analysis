#pragma once

class Cell {
public:
    enum class CellType { Floor, Wall, Slow };

    Cell();
    explicit Cell(CellType t);

    CellType Type() const;
    void SetType(CellType t);

    bool IsPassable() const;
    bool IsSlow() const;

private:
    CellType type_;
};
