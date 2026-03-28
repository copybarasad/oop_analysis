#include "cell.h"

Cell::Cell()
    : type(Cell_type::EMPTY), has_player(false), has_enemy(false), 
        has_building(false), has_ally(false){
}

Cell::Cell(Cell_type cell_type)
    : type(cell_type), has_player(false), has_enemy(false), has_building(false){
}

Cell_type Cell::get_type() const{
    return type;
}

bool Cell::get_has_player() const{
    return has_player;
}

bool Cell::get_has_enemy() const{
    return has_enemy;
}

bool Cell::get_has_building() const{
    return has_building;
}

bool Cell::is_passable() const{
    return type != Cell_type::BLOCKED && !has_building;
}

void Cell::set_type(Cell_type cell_type){
    type = cell_type;
}

void Cell::set_has_player(bool has_player){
    this->has_player = has_player;
}

void Cell::set_has_enemy(bool has_enemy){
    this->has_enemy = has_enemy;
}

void Cell::set_has_building(bool has_building){
    this->has_building = has_building;
}

bool Cell::is_empty() const{
    return !has_player && !has_enemy && !has_building;
}

bool Cell::can_move_to() const{
    return is_passable() && is_empty();
}

bool Cell::get_has_ally() const {
    return has_ally;
}

void Cell::set_has_ally(bool has_ally) {
    this->has_ally = has_ally;
}