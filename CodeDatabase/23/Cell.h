#ifndef CELL_H
#define CELL_H

enum class CellType {
    NORMAL,
    IMPASSABLE,
    SLOWING
};

class Cell {
private:
    CellType type;

public:
    Cell(CellType type = CellType::NORMAL);
    
    CellType getType() const;
    bool isPassable() const;
    bool isSlowing() const;
    void setType(CellType type);
};

#endif 
