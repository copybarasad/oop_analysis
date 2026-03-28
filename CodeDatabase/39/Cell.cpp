#include "Cell.h"

Cell::Cell()
    : terrain_(Terrain::kEmpty),
      occupant_(Occupant::kNone),
      occupant_index_(-1),
      has_trap_(false),
      trap_damage_(0) {
}

Cell::Terrain Cell::GetTerrain() const {
    return terrain_;
}

void Cell::SetTerrain(Terrain terrain) {
    terrain_ = terrain;
}

Cell::Occupant Cell::GetOccupant() const {
    return occupant_;
}

void Cell::SetOccupant(Occupant occupant) {
    occupant_ = occupant;
    if (occupant_ == Occupant::kNone) {
        occupant_index_ = -1;
    }
}

int Cell::GetOccupantIndex() const {
    return occupant_index_;
}

void Cell::SetOccupantIndex(int index) {
    occupant_index_ = index;
}

// ---- Trap methods ----

bool Cell::HasTrap() const {
    return has_trap_;
}

int Cell::GetTrapDamage() const {
    return trap_damage_;
}

void Cell::SetTrap(int damage) {
    has_trap_ = true;
    trap_damage_ = damage;
}

void Cell::ClearTrap() {
    has_trap_ = false;
    trap_damage_ = 0;
}
