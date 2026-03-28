#pragma once

#include "enums.h"

class Cell {
private:
    CellType type;
    bool hasPlayer;
    bool hasEnemy;
    bool hasBuilding;

public:
    Cell();
    
    CellType getType() const;
    void setType(CellType newType);
    
    bool hasPlayerHere() const;
    void setPlayerPresence(bool presence);
    
    bool hasEnemyHere() const;
    void setEnemyPresence(bool presence);
    
    bool hasBuildingHere() const;
    void setBuildingPresence(bool presence);
    
    bool isPassable() const;

    void setState(CellType newType, bool player, bool enemy, bool building);
};