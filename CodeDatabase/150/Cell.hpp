#ifndef CELL_H
#define CELL_H

#include "CellType.hpp"

class Cell {
private:
    CellType type_;
    bool hasPlayer_;
    bool hasBuilding_;
    bool hasEnemy_;
    bool hasAlly_;

public:
    explicit Cell(CellType type = CellType::Empty);
    
    CellType getType() const;
    void setType(CellType type);
    bool isEmpty() const;
    bool isPassable() const;
    bool isSlowing() const;
    bool hasPlayer() const;
    void setHasPlayer(bool hasPlayer);
    bool hasEnemy() const;
    void setHasEnemy(bool hasEnemy);
    bool hasBuilding() const;
    void setHasBuilding(bool hasBuilding);
    bool hasAlly() const;
    void setHasAlly(bool hasAlly);
    void clear();
};

#endif