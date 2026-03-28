#include "ally.h"
#include "field.h"
#include "enemy.h"
#include <cmath>
#include <iostream>

Ally::Ally(int pos_x, int pos_y, int hp, int dmg)
    : x(pos_x), y(pos_y), health(hp), damage(dmg), speed(1) {}

void Ally::take_damage(int dmg) {
    if (dmg > 0) health -= dmg;
    if (health < 0) health = 0;
}

void Ally::update(Field& field) {
    if (!is_alive()) return;

    Enemy* target = nullptr;
    int min_dist = 1000;
    for (const auto& enemy : field.get_enemies()) {
        if (!enemy->is_alive()) continue;
        
        int dist = abs(x - enemy->get_x()) + abs(y - enemy->get_y());
        if (dist < min_dist) {
            min_dist = dist;
            target = enemy.get();
        }
    }

    if (!target) return;

    int dx = (target->get_x() > x) ? 1 : (target->get_x() < x) ? -1 : 0;
    int dy = (target->get_y() > y) ? 1 : (target->get_y() < y) ? -1 : 0;

    int new_x = x + dx;
    int new_y = y + dy;

    if (field.is_valid_position(new_x, new_y) && !field.is_cell_occupied(new_x, new_y)) {
        x = new_x;
        y = new_y;
    }

    if (abs(x - target->get_x()) + abs(y - target->get_y()) <= 1) {
        target->take_damage(damage);
        cout << "Ally attacks enemy for " << damage << "!\n";
    }
}

int Ally::get_x() const {
    return x;
}

int Ally::get_y() const {
    return y;
}

bool Ally::is_alive() const {
    return health > 0;
}

char Ally::get_symbol() const {
    return is_alive() ? 'A' : 'a';
}