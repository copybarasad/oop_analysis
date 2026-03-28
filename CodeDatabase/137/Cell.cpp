#include "Cell.h"

Cell::Cell()
    : type_(CellType::NORMAL),
      has_player_(false),
      has_enemy_(false),
      has_enemy_building_(false),
      has_enemy_tower_(false),
      has_ally_(false),
      has_trap_(false),
      trap_damage_(0) {}

Cell::Cell(CellType type)
    : type_(type),
      has_player_(false),
      has_enemy_(false),
      has_enemy_building_(false),
      has_enemy_tower_(false),
      has_ally_(false),
      has_trap_(false),
      trap_damage_(0) {}

CellType Cell::GetType() const {
    return type_;
}

void Cell::SetType(CellType type) {
    type_ = type;
}

bool Cell::IsPassable() const {
    return type_ != CellType::WALL;
}

bool Cell::HasPlayer() const {
    return has_player_;
}

bool Cell::HasEnemy() const {
    return has_enemy_;
}

bool Cell::HasEnemyBuilding() const {
    return has_enemy_building_;
}

bool Cell::HasEnemyTower() const {
    return has_enemy_tower_;
}

bool Cell::HasAlly() const {
    return has_ally_;
}

bool Cell::HasTrap() const {
    return has_trap_;
}

int Cell::GetTrapDamage() const {
    return trap_damage_;
}

void Cell::SetPlayer(bool value) {
    has_player_ = value;
}

void Cell::SetEnemy(bool value) {
    has_enemy_ = value;
}

void Cell::SetEnemyBuilding(bool value) {
    has_enemy_building_ = value;
}

void Cell::SetEnemyTower(bool value) {
    has_enemy_tower_ = value;
}

void Cell::SetAlly(bool value) {
    has_ally_ = value;
}

void Cell::SetTrap(bool value, int damage) {
    has_trap_ = value;
    trap_damage_ = damage;
}
