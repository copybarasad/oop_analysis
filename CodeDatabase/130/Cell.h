#ifndef CELL_H
#define CELL_H

#include "GameTypes.h"

class Cell {
private:
    CellType type;
    bool hasEnemy;
    bool hasBuilding;

public:
    Cell();
    explicit Cell(CellType type);

    CellType getType() const;
    bool hasEnemyOnCell() const;
    bool hasBuildingOnCell() const;
    bool isEmpty() const;
    bool isPassable() const;

    void setType(CellType newType);
    void setEnemyPresence(bool presence);
    void setBuildingPresence(bool presence);
    void clearEntities();
};

#endif