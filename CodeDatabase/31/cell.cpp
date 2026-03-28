#include "cell.hpp"
#include "entities/entity.hpp"
#include <stdexcept>

Cell::Cell(CellType type) : type(type), entity(nullptr) {}

CellType Cell::get_type() const {
    return type;
}

void Cell::set_type(CellType type) {
    this->type = type;
}

void Cell::set_entity(Entity *entity) {
    if (trap_damage && entity != nullptr && entity->get_team() == Team::Enemy) {
        entity->take_damage(trap_damage);
        trap_damage = 0;
    }
    this->entity = entity;
}

void Cell::set_trap(int damage) {
    trap_damage = damage;
}

bool Cell::has_trap () const {
    return trap_damage > 0;
}

bool Cell::has_entity() const {
    return this->entity != nullptr;
}

EntityType Cell::get_entity_type() {
    if (has_entity() == false) throw std::logic_error("get nullptr entity type");
    else return entity->get_entity_type();
}

Entity &Cell::get_entity() {
    if (has_entity() == false) throw std::logic_error("get nullptr entity");
    else return *entity;
}

int Cell::get_trap_damage() const {
    return trap_damage;
}

void Cell::set_trap_damage(int damage) {
    trap_damage = damage;
}