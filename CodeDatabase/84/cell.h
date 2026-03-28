#ifndef CELL_H
#define CELL_H

enum class CellType {
    Empty,
    Wall
};

class Cell {
private:
    CellType type_;

public:
    Cell(CellType type = CellType::Empty);
    bool is_passable() const;
    CellType get_type() const;
    void set_type(CellType type);
};

#endif