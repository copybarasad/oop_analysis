#ifndef CELL_H
#define CELL_H

enum class CellType { kEmpty, kImpassable, kSlowing };

class Cell {
public:
    Cell();
    CellType getType() const;
    void setType(CellType type);

private:
    CellType type_;
};

#endif