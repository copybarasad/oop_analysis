#include "tower.h"
#include "field.h"
#include "player.h"
#include <iostream>
#include <cmath>

Tower::Tower(int pos_x, int pos_y, int dmg, int rad, int cd)
    : x(pos_x), y(pos_y), attack_damage(dmg), attack_radius(rad),
      cooldown(cd), current_cooldown(0) {}

void Tower::update() {
    if (current_cooldown > 0) {
        current_cooldown--;
    }
}

void Tower::try_attack(Field& field) {
    if (current_cooldown > 0) {
        return;
    }

    Player* player = field.get_player();
    if (!player || !player->is_alive()) return;

    int player_x = field.get_player_x();
    int player_y = field.get_player_y();
    int dist = abs(x - player_x) + abs(y - player_y);

    if (dist <= attack_radius) {
        player->take_damage(attack_damage);
        current_cooldown = cooldown;
        cout << "Tower at (" << x << "," << y << ") fires! Player takes " << attack_damage << " damage.\n";
    }
}

int Tower::get_x() const {
    return x;
}

int Tower::get_y() const {
    return y;
}

bool Tower::is_active() const {
    return current_cooldown == 0;
}

std::string Tower::get_symbol() const {
    return "X";
}

int Tower::get_damage() const{
    return attack_damage;
}
int Tower::get_radius() const{
    return attack_radius;
}
int Tower::get_cooldown() const{
    return cooldown;
}
int Tower::get_current_cooldown() const{
    return current_cooldown;
}

void Tower::set_current_cooldown(int new_cooldown) {
    current_cooldown = new_cooldown;
}