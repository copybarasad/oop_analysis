#ifndef CELL_H
#define CELL_H

enum CellType {
    EMPTY,
    WALL,
    TOWER_BASE
};

class Cell {
private:
    CellType type;

public:
    explicit Cell(CellType cellType = EMPTY);
    bool isPassable() const;
    CellType getType() const;
    void setType(CellType newType);
};

#endif

