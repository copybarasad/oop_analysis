#ifndef OOP_CELL_H
#define OOP_CELL_H

enum class CellType { EMPTY, OBSTACLE, SLOW, TRAP };

class Cell {
    CellType type;
    bool passable;
    int trap_damage;

public:
    explicit Cell(CellType type = CellType::EMPTY);

    CellType get_type() const;
    bool is_passable() const;

    void set_type(CellType new_type);
    int get_trap_damage() const;
    void set_trap_damage(int damage);

};

#endif //OOP_CELL_H