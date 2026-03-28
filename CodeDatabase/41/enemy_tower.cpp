#include "enemy_tower.h"
#include "game_field.h"
#include "player.h"
#include "spell_direct_damage.h"
#include <iostream>
#include <cmath>

Enemy_tower::Enemy_tower(const std::string& building_name, int pos_x, int pos_y, Game_field* game_field,
                        int health, int range, int damage, int cooldown)
            : Building(building_name, pos_x, pos_y, game_field, 0, health),
            attack_range(range), attack_damage(damage), attack_cooldown(cooldown),
            current_cooldown(0), can_attack(true){
}

void Enemy_tower::update(){
    if (destroyed) return;
    if (current_cooldown > 0){
        current_cooldown--;
        if (current_cooldown == 0){
            can_attack = true;
            std::cout << name << " готова к атаке!\n";
        }
    }
    if (can_attack && can_attack_player()){
        perform_attack();
    }
}

bool Enemy_tower::can_attack_player() const{
    if (!field) return false;
    auto player_pos = field->get_player_position();
    int player_x = player_pos.first;
    int player_y = player_pos.second;
    double distance = std::sqrt(std::pow(player_x - x, 2) + std::pow(player_y - y, 2));
    return distance <= attack_range;
}

void Enemy_tower::perform_attack(){
    if (!field) return;
    auto player_pos = field->get_player_position();
    int player_x = player_pos.first;
    int player_y = player_pos.second;
    std::cout << name << " атакует игрока ослабленным магическим снарядом!\n";
    
    Player* player = field->get_player();
    if (player){
        int weak_damage = attack_damage / 2;
        player->take_damage(weak_damage);
        std::cout << "Башня наносит " << weak_damage << " урона игроку!\n";
    } else{
        std::cout << "Башня не нашла цель для атаки!\n";
    }
    can_attack = false;
    current_cooldown = attack_cooldown;
    std::cout << name << " перезаряжается! Следующая атака через " << attack_cooldown << " ходов.\n";
}