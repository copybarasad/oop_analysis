#include "spell_trap.h"
#include "player.h"
#include "game_field.h"
#include <cmath>
#include <iostream>

Spell_trap::Spell_trap(int trap_damage)
    : Spell_card("Магическая ловушка", "Размещает ловушку на поле", SPELL_TRAP_COST, SPELL_TRAP_RANGE),
    damage(trap_damage){
}

bool Spell_trap::can_cast(const Player& player, int target_x, int target_y, const Game_field& field) const{
    auto player_pos = field.get_player_position();
    double distance = std::sqrt(std::pow(target_x - player_pos.first, 2) + std::pow(target_y - player_pos.second, 2));
    if (distance > range){
        std::cout << "Цель слишком далеко! Дистанция: " << distance << ", максимальная: " << range << "\n";
        return false;
    }
    if (!field.is_valid_position(target_x, target_y) || !field.get_cell(target_x, target_y).can_move_to()){
        std::cout << "Нельзя разместить ловушку в этой клетке!\n";
        return false;
    }
    return true;
}

void Spell_trap::cast(Player& player, int target_x, int target_y, Game_field& field){
    std::cout << player.get_name() << " применяет " << name << "!\n";
    field.place_trap(target_x, target_y, damage);
    std::cout << "Ловушка размещена в позиции (" << target_x << "," << target_y << ")\n";
}

Spell_card* Spell_trap::clone() const{
    return new Spell_trap(*this);
}

void Spell_trap::enhance() {
    damage += 5;
    enhanced = true;
    std::cout << name << " усилено! Новый урон: " << damage << "\n";
}

Target_type Spell_trap::get_target_type() const {
    return Target_type::AREA;
}