#ifndef CELL_H
#define CELL_H

#include "CellType.h"

class Cell {
private:
    CellType cellType;
    
public:
    Cell();
    
    CellType getType() const;
    void setType(CellType newType);
    
    bool isEmpty() const;
    bool isObstacle() const;
    bool isSlow() const;
    
    void clear();
};

#endif