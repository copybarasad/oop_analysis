#ifndef CELL_H
#define CELL_H

enum class CellType {
    EMPTY,
    WALL,
    PLAYER,
    ENEMY
};

class Cell {
private:
    CellType type;
    bool isPassable;

public:
    Cell();
    explicit Cell(CellType cellType);
    
    bool getIsPassable() const;
    CellType getType() const;
    void setType(CellType cellType);
};

#endif