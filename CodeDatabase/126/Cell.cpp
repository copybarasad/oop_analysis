#include "../../headers/field/Cell.h"
#include <string>

using namespace std;

Cell::Cell() : is_occupied(false), has_player(false), has_enemy(false), is_blocked(false), has_tower(false), has_trap(false) {}

void Cell::clear() {
    is_occupied = false;
    has_player = false;
    has_enemy = false;
    has_tower = false;
    has_trap = false;
}

void Cell::setPlayer() {
    is_occupied = true;
    has_player = true;
    has_enemy = false;
    has_tower = false;
}

void Cell::setEnemy() {
    is_occupied = true;
    has_player = false;
    has_enemy = true;
    has_tower = false;
}

void Cell::setTower() {
    is_occupied = true;
    has_player = false;
    has_enemy = false;
    has_tower = true;
}

void Cell::setTrap() {
    has_trap = true;
}

void Cell::removeTrap() {
    has_trap = false;
}

void Cell::setBlocked(bool blocked){
    is_blocked = blocked;
}

bool Cell::getIsOccupied() const { return is_occupied; }

bool Cell::getHasPlayer() const { return has_player; }

bool Cell::getHasEnemy() const { return has_enemy; }

bool Cell::getHasTower() const { return has_tower; }

bool Cell::getHasTrap() const { return has_trap; }

bool Cell::getIsBlocked() const { return is_blocked; }

string Cell::serialize() const {
    return string(is_blocked ? "1" : "0");
}

void Cell::deserialize(char data) {
    is_blocked = (data == '1');
    clear();
}