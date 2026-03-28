#ifndef CELL_H
#define CELL_H

enum Type_cell {
    EMPTY,
    PLAYER,
    ENEMY,
    PIT,
    BARRACK,
    SALOON,
    COWBOYTOWER,
    TRAP,
    JHONNYGHOST
};

class Cell {
private:
    Type_cell type;
    bool can_go;
public:
    Cell(Type_cell cellType = EMPTY, bool canWalk = true);
    
    Type_cell getType() const;
    bool isWalkable() const;
    
    void setType(Type_cell newType);
    void setWalkable(bool canWalk);
    
    char getSymbol() const;
};

#endif