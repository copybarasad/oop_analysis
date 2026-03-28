#include "game_cell.h"

game_cell::game_cell() : has_player(false), has_enemy(false), has_trap(false), has_tower(false), trap_damage(0)
{}

bool game_cell::hasPlayer() const {
    return has_player;
}

bool game_cell::hasEnemy() const {
    return has_enemy;
}

bool game_cell::hasTrap() const {
    return has_trap;
}

bool game_cell::hasTower() const {
    return has_tower;
}

int game_cell::getTrapDamage() const {
    return trap_damage;
}

void game_cell::setPlayer(bool does_has_player) {
    has_player = does_has_player;
}

void game_cell::setEnemy(bool does_has_enemy) {
    has_enemy = does_has_enemy;
}

void game_cell::setTrap(bool does_has_trap, int damage) {
    has_trap = does_has_trap;
    trap_damage = damage;
}

void game_cell::setTower(bool does_has_tower) {
    has_tower = does_has_tower;
}

bool game_cell::isEmpty() const {
    return !has_player && !has_enemy && !has_trap && !has_tower;
}
