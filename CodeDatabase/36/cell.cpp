#include "cell.h"

Cell::Cell() : type_(CellType::Empty), enemy_index_(-1), ally_index_(-1), trap_damage_(0) {}

CellType Cell::GetType() const { return type_; }
int Cell::GetEnemyIndex() const { return enemy_index_; }
int Cell::GetAllyIndex() const { return ally_index_; }
int Cell::GetTrapDamage() const { return trap_damage_; }

void Cell::SetType(CellType t) { type_ = t; }
void Cell::SetEnemyIndex(int idx) { enemy_index_ = idx; }
void Cell::SetAllyIndex(int idx) { ally_index_ = idx; }
void Cell::SetTrapDamage(int dmg) { trap_damage_ = dmg; }

bool Cell::IsPassable() const {
    return (type_ == CellType::Empty || type_ == CellType::Slow || type_ == CellType::Trap);
}

bool Cell::ContainsEnemy() const { return type_ == CellType::Enemy && enemy_index_ >= 0; }
bool Cell::ContainsAlly() const { return type_ == CellType::Ally && ally_index_ >= 0; }

void Cell::Clear() {
    type_ = CellType::Empty;
    enemy_index_ = -1;
    ally_index_ = -1;
    trap_damage_ = 0;
}