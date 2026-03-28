#include "trap.h"
#include "game_field.h"
#include <iostream>

Trap::Trap(int pos_x, int pos_y, int trap_damage)
    : x(pos_x), y(pos_y), damage(trap_damage), active(true){
}

int Trap::get_x() const{
    return x;
}

int Trap::get_y() const{
    return y;
}

int Trap::get_damage() const{
    return damage;
}

bool Trap::is_active() const{
    return active;
}

void Trap::trigger(){
    active = false;
    std::cout << "Ловушка срабатывает!";
}

void Trap::update(Game_field& field){
    if (!active) return;
    auto enemies = field.get_enemy_positions();
    for (size_t i = 0; i < enemies.size(); ++i) { 
        if (enemies[i].first == x && enemies[i].second == y) {
            trigger();
            std::cout << "Ловушка срабатывает на враге в позиции (" << x << "," << y << ")!\n";
            field.damage_enemy_at_position(x, y, damage);  
            break;
        }
    }
}