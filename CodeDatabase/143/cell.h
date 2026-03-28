#ifndef CELL_H
#define CELL_H

#include <iostream>

enum class CellType {
    EMPTY,
    PLAYER,
    ENEMY
};

class Cell {
public:
    Cell();
    CellType getType() const;
    void setType(CellType type);
    bool isEmpty() const;
    void save(std::ostream& out) const;
    void load(std::istream& in);

private:
    CellType cellType;
};

#endif



