#ifndef CELL_H
#define CELL_H


class Cell {
private:
    bool isPassable;

public:
    Cell();
    Cell(bool passable);

    bool getIsPassable() const { return isPassable; }
    bool setIsPassable(bool passable) { return isPassable = passable; }
};


#endif //CELL_H