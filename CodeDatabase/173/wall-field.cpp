#include <string>

#include "../combattable.hpp"
#include "wall-field.hpp"

WallField::WallField() {}
WallField::WallField(const WallField &field) {}
Field *WallField::clone() { return new WallField(*this); }

void WallField::attack(Combattable *attacker) {}
void WallField::build(Building *building) {}
void WallField::deoccupy() {}
void WallField::occupy(Creature *occupier) {}
void WallField::damage(int damage) {}
void WallField::placeTrap(Trap *trap) {}
bool WallField::canAttack() { return false; }
bool WallField::canBuild() { return false; }
bool WallField::canOccupy() { return false; }
bool WallField::canPlaceTrap() { return false; }
bool WallField::isOccupiedByEnemy() { return false; }

void WallField::update() {}

std::string WallField::chr() { return "█"; };
Color WallField::color() { return Color::white(); }

bytedump WallField::dump() {
    bytedump bytes;
    bytes.push_back(static_cast<std::byte>(WALL_FIELD_BYTE));
    return bytes;
}
