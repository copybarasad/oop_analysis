#include "cell.h"

Cell::Cell(): type(cell_type::usual), occupied(nullptr) {};

Cell::Cell(cell_type type, Character* occupied): type(type), occupied(occupied) {};

Cell::Cell(const Cell& cell): type(cell.type), occupied(nullptr){ // copyrovanie
    if(cell.occupied){
        if(cell.occupied->get_type() == character_type::hero){
            occupied = new Hero(*static_cast<Hero*>(cell.occupied));
        }else if(cell.occupied->get_type() == character_type::ally){
            occupied = new Ally(*static_cast<Ally*>(cell.occupied));
        }else if(cell.occupied->get_type() == character_type::enemy){
            occupied = new Enemy(*static_cast<Enemy*>(cell.occupied));
        }else if(cell.occupied->get_type() == character_type::tower){
            occupied = new Tower(*static_cast<Tower*>(cell.occupied));
        }else if(cell.occupied->get_type() == character_type::base){
            occupied = new Base(*static_cast<Base*>(cell.occupied));
        }
    }
};

Cell& Cell::operator=(const Cell& cell){
    if(this == &cell) return *this;
        
    delete occupied;

    type = cell.type;
    occupied = nullptr;

    if(cell.occupied){
        if(cell.occupied->get_type() == character_type::hero){
            occupied = new Hero(*static_cast<Hero*>(cell.occupied));
        }else if(cell.occupied->get_type() == character_type::ally){
            occupied = new Ally(*static_cast<Ally*>(cell.occupied));
        }else if(cell.occupied->get_type() == character_type::enemy){
            occupied = new Enemy(*static_cast<Enemy*>(cell.occupied));
        }else if(cell.occupied->get_type() == character_type::tower){
            occupied = new Tower(*static_cast<Tower*>(cell.occupied));
        }else if(cell.occupied->get_type() == character_type::base){
            occupied = new Base(*static_cast<Base*>(cell.occupied));
        }
    }
    return *this;
};

Cell::Cell(Cell&& cell) noexcept: type(cell.type), occupied(cell.occupied) { // peremeschtschenie
    cell.type = cell_type::usual;
    cell.occupied = nullptr;
};

Cell& Cell::operator=(Cell&& cell) noexcept {
    if(this == &cell) return *this;

    type = cell.type;
    occupied = cell.occupied;

    cell.type = cell_type::usual;
    cell.occupied = nullptr;

    return *this;
};

Cell::~Cell(){
    delete occupied;
};

void Cell::set_trap(unsigned int damage){
    if(type == cell_type::impassable || occupied) return;
    trap = true;
    this->damage = damage;
};

unsigned int Cell::use_trap(){
    if(!trap) return 0;
    unsigned int damage = this->damage;
    trap = false;
    this->damage = 0;
    return damage;
};