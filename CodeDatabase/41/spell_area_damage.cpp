#include "spell_area_damage.h"
#include "player.h"
#include "game_field.h"
#include "constants.h"
#include <cmath>
#include <iostream>

Spell_area_damage::Spell_area_damage(int spell_damage, int size, bool is_enhanced) 
    : Spell_card("Огненный взрыв", "Наносит урон по области", 
                SPELL_AREA_DAMAGE_COST, 
                SPELL_AREA_DAMAGE_RANGE),
      damage(spell_damage), area_size(is_enhanced ? size + 1 : size), enhanced(is_enhanced){
}

bool Spell_area_damage::can_cast(const Player& player, int target_x, int target_y, const Game_field& field) const{
    (void)player; 
    auto player_pos = field.get_player_position();
    double distance = std::sqrt(std::pow(target_x - player_pos.first, 2) + std::pow(target_y - player_pos.second, 2));
    
    if (distance > range){
        std::cout << "Цель слишком далеко! Дистанция: " << distance << ", максимальная: " << range << "\n";
        return false;
    }
    return true;
}


void Spell_area_damage::cast(Player& player, int target_x, int target_y, Game_field& field){
    try {       
        std::cout << player.get_name() << " применяет " << name << "!\n";
        field.perform_area_attack(player.get_name(), target_x, target_y, damage, area_size);
    } catch (const std::exception& e) {
        std::cout << "Ошибка при применении заклинания: " << e.what() << std::endl;
    }
}

Spell_card* Spell_area_damage::clone() const{
    return new Spell_area_damage(*this);
}

void Spell_area_damage::enhance(){
    area_size++;
    enhanced = true;
    std::cout << name << " усилено! Новый размер области: " << area_size << "x" << area_size << "\n";
}

Target_type Spell_area_damage::get_target_type() const {
    return Target_type::AREA;
}