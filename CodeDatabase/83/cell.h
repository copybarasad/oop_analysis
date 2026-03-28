#ifndef CELL_H
#define CELL_H

class Cell {
public:
    Cell();
    bool canPass() const;
    void setPassable(bool canPass);

private:
    bool passable;
};

#endif