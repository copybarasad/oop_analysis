#ifndef CELL_H
#define CELL_H

#include "Types.h"

class Cell {
private:
    Coords coords;
    bool walkable;

public:
    Cell(int x, int y, bool walkable);

    bool isWalkable() const {
        return walkable;
    }
    void setWalkable(bool newVal) {
        walkable = newVal;
    }

    json serialize() const;
    void deserialize(const json& j);
};

#endif // CELL_H
