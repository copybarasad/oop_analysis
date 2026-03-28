#include "cell.h"

Cell::Cell(int pos_x, int pos_y)
    : is_empty_cell(true), position_x(pos_x), position_y(pos_y) {}

bool Cell::is_empty() const{
    return is_empty_cell;
}

int Cell::get_position_x() const {return position_x;}
int Cell::get_position_y() const {return position_y;}

void Cell::set_empty(bool value) {is_empty_cell = value;}

bool Cell::can_pass() const{
    return is_empty_cell;  
}

void Cell::clear(){
    is_empty_cell = true;
}