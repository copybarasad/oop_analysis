#include "cell.h"

Cell::Cell(CellType type) : type_(type) {}

bool Cell::is_passable() const{
    return type_ == CellType::Empty;
}

CellType Cell::get_type() const{
    return type_;
}

void Cell::set_type(CellType type){
    type_ = type;
}