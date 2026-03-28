#include "cell.hpp"

Cell::Cell(int x, int y, TypeCell type_cell) : x(x), y(y), type_cell(type_cell), entity(nullptr) {};
Cell::~Cell(){
    entity = nullptr;
}

void Cell::clear_entity(){
    entity = nullptr;
}

int Cell::get_x() {return x;};
int Cell::get_y() {return y;};
void Cell::get_coord(int& x, int& y) {x = this->x, y = this->y;};

Entity* Cell::get_entity() const {return entity;};
int Cell::set_entity(Entity* entity) {
    if(this->entity && this->entity->is_alive()){
        if (entity->get_fraction() == Fraction::BLUE) std::cout << "cell is occuped!" << std::endl;
        return 1;
    } else if (type_cell == TypeCell::PASSABLE){
        if (entity->get_fraction() == Fraction::BLUE) std::cout << "cell is PASSABLE" << std::endl;
        return 1;
    }
    this->entity = entity;

    return 0;
}
int Cell::move_entity(Entity* entity, Cell& target_cell){
    Entity* target_cell_entity = target_cell.get_entity();

    if (target_cell_entity){
        if (entity->get_fraction() == target_cell_entity->get_fraction()) return 0;

        if (entity->attack(*target_cell_entity)){
            return 0;
        } else{
            if(entity->get_symbol() == 'P'){
                dynamic_cast<Player*>(entity)->add_points(1);
            }
        }
    }
    target_cell.set_entity(entity);
    this->entity = nullptr;

    return 0;
}

TypeCell Cell::get_type_cell() const {return type_cell;};
void Cell::set_type_cell(TypeCell type_cell) {this->type_cell = type_cell;};