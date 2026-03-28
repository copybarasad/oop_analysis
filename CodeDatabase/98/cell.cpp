#include "cell.h"

Cell::Cell(): currentcell(CellType::PASSABLE) {} //по умолчанию все проходимое, в классе поле будет замена

CellType Cell::GetCellType() const{
    return currentcell;
}

void Cell::SwitchCellType(CellType new_type){
    currentcell = new_type;
}
bool Cell::IsPassable() const{
    return currentcell == CellType::PASSABLE;
}
bool Cell::IsBlocked() const{
    return currentcell == CellType::BLOCKED;
}