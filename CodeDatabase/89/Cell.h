#ifndef CELL_H
#define CELL_H

enum class CellType {
    Empty,
    Impassable,
    Player,
    Enemy,
    Tower,
    Trap
};

class Cell {
public:
    Cell();
    void setType(CellType type);
    CellType getType() const;
    bool isPassable() const;

private:
    CellType type;
};

#endif