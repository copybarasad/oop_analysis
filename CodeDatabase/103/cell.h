#pragma once

#include <iostream>
#include "cell_type.h"


class Cell {
private:
    CellType type;
    int hasTrapDamage;
public:
    Cell();
    CellType getType() const;
    int getTrapDamage() const;
    bool isPassable() const;
    void setTrapDamage(int dmg);
    void setType(CellType newType);
};