#pragma once
#include <vector>
#include <cstdlib>
#include "../../saving/Saving.h"
#include "../utils/Point.h"
#include "Cell.h"

class Field{
private:
    std::vector<std::vector<Cell>> cells;
    int height;
    int width;
public:
    Field() ;
    Field(int height, int width);
    ~Field();

    void move_player(Point source, Point destination);
    bool move_enemy(Point source, Point destination);

    void remove_enemy(Point pos) ;
    void set_enemy_at(Point pos) ;
    void set_trap_at(Point pos, bool value);

    bool in_the_field(Point pos);
    bool can_shift_left(Point pos);
    bool can_shift_down(Point pos);
    bool can_shift_up(Point pos);
    bool can_shift_right(Point pos);

    bool has_enemy_at(Point pos) ;
    bool is_impassible_at(Point pos);
    bool is_trap_at(Point pos);

    Point get_sizes();
    Cell get_cell(int i, int j); //для синхронизации с графическим полем

    Saving::FieldData serialize() const;
    void deserialize(const Saving::FieldData& data);

    Field(const Field& copy);
    Field(Field&& moved);
    Field& operator=(Field&& moved);
    Field& operator=(const Field& copy);
};

