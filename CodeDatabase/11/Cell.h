#ifndef CELL_H
#define CELL_H

enum class CellType {
    EMPTY = '.',
    PLAYER = 'P',
    ENEMY = 'E',
    WALL = '#',
    SLOW = 'S',   
    SPAWNER = 'X',
    ALLY = 'A',
    ENEMY_TOWER = 'T',
    TRAP = '^'
};

class Cell {
public:
    Cell();
    CellType getType() const;
    void setType(CellType type);

private:
    CellType type;
};
#endif