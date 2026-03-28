#include "enemy.h"
#include "game_field.h"
#include "player.h"
#include "constants.h"
#include "enums.h"
#include <iostream>
#include <cmath>

Enemy::Enemy(const std::string& enemy_name, Game_field* game_field)
    : name(enemy_name), 
      health(DEFAULT_ENEMY_HEALTH),      
      max_health(DEFAULT_ENEMY_HEALTH),   
      damage(DEFAULT_ENEMY_DAMAGE),        
      is_alive(true), 
      field(game_field){
}

std::string Enemy::get_name() const{
    return name;
}

int Enemy::get_health() const{
    return health;
}

int Enemy::get_damage() const{
    return damage;
}

bool Enemy::get_is_alive() const{
    return is_alive;
}

void Enemy::set_health(int new_health){
    if (new_health <= max_health){
        health = new_health;
    } else{
        health = max_health;
    }
    if (health <= 0){
        health =0;
        is_alive = false;
        std::cout << name << " побежден!\n";
    }
}

void Enemy::take_damage(int damage_amount){
    if (damage_amount > 0 && is_alive){
        health -= damage_amount;
        std::cout << name << " получает " << damage_amount << " урона!\n";
        if (health <= 0){
            health = 0;
            is_alive = false;
            std::cout << name << " побежден!\n";
        }
    }
}

bool Enemy::make_move(Player& player){
    if (!is_alive){
        return false;
    }
    auto player_pos = field->get_player_position();
    auto enemy_pos = field->get_enemy_position(this);
    if (enemy_pos.first == -1) return false;

    int player_x = player_pos.first;
    int player_y = player_pos.second;
    int enemy_x = enemy_pos.first;
    int enemy_y = enemy_pos.second;

    int dx_attack = std::abs(enemy_x - player_x);
    int dy_attack = std::abs(enemy_y - player_y);
    if (dx_attack <= 1 && dy_attack <= 1 && (dx_attack + dy_attack) > 0) {
        std::cout << name << " атакует " << player.get_name() << " и наносит " << damage << " урона!\n";
        player.take_damage(damage); 
        return true;
    }

    int dx_move = player_x - enemy_x;
    int dy_move = player_y - enemy_y;
    int move_x = 0, move_y = 0;
    if (std::abs(dx_move) > std::abs(dy_move)){
        move_x = (dx_move > 0) ? 1 : -1;
    } else{
        move_y = (dy_move > 0) ? 1 : -1;
    }
    int new_x = enemy_x + move_x;
    int new_y = enemy_y + move_y;

    int enemy_index = field->find_enemy_index(this);
    if (enemy_index != -1){
        bool moved = field->move_enemy(enemy_index, new_x, new_y);
        if (moved){
            std::cout << name << " двигается к игроку\n";
        }
        return moved;
    }
    return false;
}

void Enemy::display_status() const {
    auto pos = field->get_enemy_position(this);

    std::cout << "\n Статус врага \n";
    std::cout << "Имя: " << name << "\n";
    std::cout << "Здоровье: " << health << "/" << max_health << "\n";
    std::cout << "Урон: " << damage << "\n";
    if (pos.first != -1) {
        std::cout << "Позиция: (" << pos.first << ", " << pos.second << ")\n";
    }
    std::cout << "Состояние: " << (is_alive ? "Жив" : "Мертв") << "\n";
}

