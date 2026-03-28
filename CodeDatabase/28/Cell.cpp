#include "Cell.h"

Cell::Cell(int x, int y) : x_pos(x), y_pos(y) {}

void Cell::set_passable(bool value) { passable = value; }
void Cell::set_player(Player* player) { player_on_cell = player; }
void Cell::set_enemy(Enemy* enemy) { enemy_on_cell = enemy; }
void Cell::set_building(EnemyBuilding* building) { building_on_cell = building; }
void Cell::set_enemy_tower(EnemyTower* tower) { enemy_tower_on_cell = tower; }
void Cell::set_slow(bool value) { slow = value; }
void Cell::set_x(int x) { x_pos = x; }
void Cell::set_y(int y) { y_pos = y; }

bool Cell::is_slow() const { return slow; }
bool Cell::is_passable() const { return passable; }
bool Cell::is_occupied() const { return player_on_cell != nullptr || enemy_on_cell != nullptr || building_on_cell != nullptr || enemy_tower_on_cell != nullptr; }
bool Cell::is_occupied_by_player() const { return player_on_cell != nullptr; }
bool Cell::is_trap() const { return trap_on_cell; }


Player* Cell::get_player() const { return player_on_cell; }
Enemy* Cell::get_enemy() const { return enemy_on_cell; }
EnemyBuilding* Cell::get_building() const { return building_on_cell; }

int Cell::get_x() const { return x_pos; }
int Cell::get_y() const { return y_pos; }

void Cell::place_trap(int damage) {
    trap_on_cell = true;
    trap_damage = damage;
}

int Cell::get_trap_damage() {
    int temp_dmg = trap_damage;
    trap_damage = 0;
    trap_on_cell = false;
    return temp_dmg;
}

int Cell::get_trap_damage_value() const { return trap_damage; }

char Cell::get_symbol() const {
    if (player_on_cell) return 'P';     // Player
    if (enemy_on_cell) return 'E';      // Enemy
    if (building_on_cell) return 'B';   // Building
    if (enemy_tower_on_cell) return 'T';// Enemy Tower
    if (!passable) return '#';          // Wall
    if (slow) return '~';               // Slow cell
    if (trap_on_cell) return 'x';
    return '.';                         // Empty
}