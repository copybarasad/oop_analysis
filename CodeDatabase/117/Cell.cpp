#include "Cell.h"

Cell::Cell(CellType cellType){
    this->cellType = cellType;
    this->busysness = false;
}

Cell::Cell():Cell(DEFAULT_CELL_TYPE){}

CellType Cell::getCellType() const{
    return this->cellType;
}

void Cell::setCellType(CellType cellType){
    if(this->isFree()){
        this->cellType = cellType;
    }
}

bool Cell::isPass() const{
    return (this->cellType != CellType::Wall);
}

void Cell::setBusysness(bool busysness){
    if(this->isPass()){
        this->busysness = busysness;
    }
}

bool Cell::isFree() const{
    return (!this->busysness && this->isPass());
}

bool Cell::getBusysness() const{
    return this->busysness;
}