#include "spell_direct_damage.h"
#include "player.h"
#include "game_field.h"
#include "constants.h"
#include <cmath>
#include <iostream>

Spell_direct_damage::Spell_direct_damage(int spell_damage, bool is_enhanced) 
    : Spell_card("Огненный шар", "Наносит прямой урон врагу или зданию", 
                SPELL_DIRECT_DAMAGE_COST, 
                is_enhanced ? SPELL_DIRECT_DAMAGE_RANGE + 1 : SPELL_DIRECT_DAMAGE_RANGE),
      damage(spell_damage), enhanced(is_enhanced) {
}

bool Spell_direct_damage::can_cast(const Player& player, int target_x, int target_y, const Game_field& field) const{
    (void)player;
    auto player_pos = field.get_player_position();
    double distance = std::sqrt(std::pow(target_x - player_pos.first, 2) + std::pow(target_y - player_pos.second, 2));
    if (distance > range){
        std::cout << "Цель слишком далеко! Дистанция: " << distance << ", максимальная: " << range << "\n";
        return false;
    }
    if (!field.is_enemy_or_building_at(target_x, target_y)){
        std::cout << "В указанной клетке нет врага или здания!\n";
        return false;
    }
    return true;
}

void Spell_direct_damage::cast(Player& player, int target_x, int target_y, Game_field& field){
    std::cout << player.get_name() << " применяет " << name << "!\n";
    field.perform_spell_attack(player.get_name(), target_x, target_y, damage);
}

Spell_card* Spell_direct_damage::clone() const{
    return new Spell_direct_damage(*this);
}

void Spell_direct_damage::enhance(){
    range++;
    enhanced = true;
    std::cout << name << " усилено! Новый радиус: " << range << "\n";
}

Target_type Spell_direct_damage::get_target_type() const {
    return Target_type::ENEMY;
}