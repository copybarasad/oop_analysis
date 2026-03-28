#include "PositionField.h"

Position_field::Position_field() : HasPlayer(false), HasEnemy(false), Blockedpos(false) {}

void Position_field::SetPlayer(bool value) { HasPlayer = value; }
void Position_field::SetEnemy(bool value) { HasEnemy = value; }
void Position_field::SetBlock(bool value) { Blockedpos = value; }
void Position_field::SetTrap(bool value) { HasTrap = value; }
void Position_field::SetTower(bool value) { HasTower = value; }

bool Position_field::isPlayer() const { return HasPlayer; }
bool Position_field::isEnemy() const { return HasEnemy; }
bool Position_field::isBlock() const { return Blockedpos; }
bool Position_field::isTrap() const { return HasTrap; }
bool Position_field::isTower() const { return HasTower; }

bool Position_field::free_pos() const {
    return !HasEnemy && !HasPlayer && !Blockedpos && !HasTower;
}

void Position_field::clear_pos() {
    HasPlayer = false;
    HasEnemy = false;
}

char Position_field::display_field() const {
    if (HasPlayer) return 'P';
    if (HasEnemy) return 'E';
    if (HasTrap) return 'X';
    if (HasTower) return 'T';
    if (Blockedpos) return '#';
    return '.';
}