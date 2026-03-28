#ifndef CELL_H
#define CELL_H

#include "celltype.h"

class Cell {
private:
    CellType type;
    
public:
    Cell();
    explicit Cell(CellType cellType);
    
    CellType getType() const;
    void setType(CellType cellType);
    
    bool isEmpty() const;
    bool isPassable() const;
    bool isSlowing() const;
};

#endif