#ifndef CELL_H
#define CELL_H

class Cell{
private:
    bool is_empty_cell;
    int position_x;
    int position_y;

public:
    Cell(int pos_x=0, int pos_y=0);

    bool is_empty() const;
    int get_position_x() const;
    int get_position_y() const;

    void set_empty(bool value);

    bool can_pass() const;

    void clear();
};

#endif