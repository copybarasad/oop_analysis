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
    bool passable;

public:
    Cell(CellType cellType = CellType::EMPTY);
    
    Cell(const Cell& other) = default;            
    Cell(Cell&& other) = default;                 
    Cell& operator=(const Cell& other) = default;
    Cell& operator=(Cell&& other) = default;      
    ~Cell() = default;                            
    
    bool isPassable() const { return passable; }
    CellType getType() const { return type; }
    void setType(CellType cellType);
    
    char getSymbol() const;
};

#endif