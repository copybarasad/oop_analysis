#include "cell.h"

Cell::Cell(Type type_)
:type(type_){}

Cell::Type Cell::get_type(){
    return type;
}

void Cell::change_type(Type t){
    type = t;
}

Cell::~Cell(){};