#include "Cell.h"
#include "GameEnt.h"

Cell::Cell(CellType typeCell, GameEnt* entityCell):type(typeCell), entity(entityCell){}

CellType Cell::getType() const {
    return type;
}

void Cell::setType(CellType typeCell){
    type = typeCell;
}

void Cell::setEntity(GameEnt* e){
    if (e == nullptr) {
        clearEnt();
    } else {
        entity = e;
    }
}

void Cell::clearType(){
    type = CellType::Empty;
}

void Cell::clearEnt(){
    entity = nullptr;
}

bool Cell::isOccupated() const{
    if (entity != nullptr || type == CellType::Wall){
        return true;
    }
    return false;
}

void Cell::printInfo(int x, int y) const {
    std::cout<<""<<std::endl;
    std::cout<<"Информация о клетке ("<<x<<", "<< y <<"):" << std::endl;
    switch (type){
        case (CellType::Empty):
            std::cout<<"Type: Empty"<<std::endl;
            break;
        case (CellType::Wall):
            std::cout<<"Type: Wall"<<std::endl;
            break;
        case (CellType::Trap):
            std::cout<<"Type: Trap"<<std::endl;
            break;
    }
    if (entity!=nullptr){
        switch (entity->getObjType()){
            case (GameEntType::Enemy):
                std::cout<<"Entity: Enemy"<<std::endl;
                break;
            case (GameEntType::Player):
                std::cout<<"Entity: Player"<<std::endl;
                break;
            case (GameEntType::Tower):
                std::cout<<"Entity: Tower"<<std::endl;
                break;
        }
    }
    std::cout<<""<<std::endl;
}

Cell Cell::deepCopy() const{
    Cell newCell;
    newCell.type = this->type;
    if (this->entity != nullptr) {
        newCell.entity = this->entity->clone();
    } else {
        newCell.entity = nullptr;
    }
    return newCell;
}