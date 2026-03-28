#include "EnemyTower.h"
#include "Player.h"
#include "Gamefield.h"
#include "Cell.h"
#include <iostream>
#include <cmath>

EnemyTower::EnemyTower(Cell* cell, int radius) : cell(cell), radius(radius) {
    cell->set_enemy_tower(this);
    this->x_pos = cell->get_x();
    this->y_pos = cell->get_y();
}

// Радиус Манхэттена (горизонтальное + вертикальное расстояние)
static bool within_radius(int sx, int sy, int tx, int ty, int r) {
    return (std::abs(sx - tx) + std::abs(sy - ty)) <= r;
}

void EnemyTower::try_attack(Player& player, Gamefield& field) {
    if (--current_timer <= 0) {
        current_timer = spell_cooldown;
        TowerAreaDamage.tower_attack(*this, field, player);
    }
}

int EnemyTower::get_x() const { return cell ? cell->get_x() : -1; }

int EnemyTower::get_y() const { return cell ? cell->get_y() : -1; }

