#pragma once
#include <iostream>
#include "GameEnt.h"


enum class CellType{
    Empty,
    Wall,
    Trap,
};

class Cell{
private:
    
    CellType type;
    GameEnt* entity;


public:
    
    Cell(CellType typeCell = CellType::Empty, GameEnt* entityCell=nullptr);

    Cell deepCopy() const;

    CellType getType() const;

    void printInfo(int x, int y) const;

    GameEnt* getEntity() const {return entity;};

    bool isOccupated() const;

    // Не константные методы

    void clearEnt();

    void clearType();

    void setEntity( GameEnt* e);

    void setType(CellType typeCell);

};