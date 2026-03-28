#include "enemy.h"
#include "player.h"
#include "playing_field.h"
#include "event_manager.h"
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>

Enemy::Enemy(int start_x, int start_y)
    : max_life(2), current_life(2), damage(1), position_x(start_x), position_y(start_y), is_alive(true) {}

void Enemy::move(int player_x, int player_y, Player& player, const Playing_field& field){
    if(!is_alive){
        return;
    }

    if (attack_player(player)) {
        return; 
    }

    int new_x = position_x;
    int new_y = position_y;

    int behavior = std::rand() % 10;
    
    if (behavior < 7) {
        if (std::abs(player_x - position_x) > std::abs(player_y - position_y)) {
            if (player_x > position_x) new_x++;
            else if (player_x < position_x) new_x--;
        } else {
            if (player_y > position_y) new_y++;
            else if (player_y < position_y) new_y--;
        }
    } else {
        int direction = std::rand() % 4;
        switch (direction){
            case 0: new_y += 1; break;
            case 1: new_y -= 1; break;
            case 2: new_x += 1; break;
            case 3: new_x -= 1; break;
        }
    }

    if (new_x >= 0 && new_x < field.get_width() && 
        new_y >= 0 && new_y < field.get_length() &&
        field.get_cell(new_x, new_y).can_pass()) {
        
        position_x = new_x;
        position_y = new_y;
    }
}

bool Enemy::attack_player(Player& player){
    int distance_x = std::abs(position_x - player.get_position_x());
    int distance_y = std::abs(position_y - player.get_position_y());
    
    if (distance_x <= 1 && distance_y <= 1) {
        std::cout << "Enemy attacks!" << std::endl;
        EventManager::trigger(EventType::DamageDealt, "Enemy", damage);
        player.taking_damage(damage);
        return true;
    }
    
    return false;
}

void Enemy::taking_damage(int damage){
    if(!is_alive){
        return;
    }
    if (damage>0){
        current_life -= damage;
        EventManager::trigger(EventType::DamageTaken, "Enemy", damage, current_life);
        if (current_life <= 0){
            current_life = 0;
            is_alive = false;
            EventManager::trigger(EventType::UnitDied, "Enemy killed");
        }
    }else{
        std::cout << "Game Over" << std::endl;
        throw std::invalid_argument("Invalid damage value");
    }
}

void Enemy::position(int new_x, int new_y) {
    position_x = new_x;
    position_y = new_y;
}

bool Enemy::get_alive() const {return is_alive;}
int Enemy::get_position_x() const {return position_x;}
int Enemy::get_position_y() const {return position_y;}
int Enemy::get_curr_life() const {return current_life;}
int Enemy::get_damage() const {return damage;}

void Enemy::increase_health(int bonus_health) {
    if (bonus_health < 0) {
        throw std::invalid_argument("Bonus health cannot be negative");
    }
    max_life += bonus_health;
    current_life += bonus_health;
}

void Enemy::increase_damage(int bonus_damage) {
    if (bonus_damage < 0) {
        throw std::invalid_argument("Bonus damage cannot be negative");
    }
    damage += bonus_damage;
}

void Enemy::set_health(int new_health) {
    if (new_health <= 0) {
        current_life = 0;
        is_alive = false;
    } else {
        current_life = std::min(new_health, max_life);
        is_alive = true;
    }
}

void Enemy::set_damage(int new_damage) {
    if (new_damage < 0) {
        throw std::invalid_argument("Damage cannot be negative");
    }
    damage = new_damage;
}

void Enemy::set_alive(bool alive) {
    is_alive = alive;
    if (!is_alive && current_life > 0) {
        current_life = 0;
    }
}