#include "enemy_tower.h"
#include "player.h"
#include "playing_field.h"
#include "enemy.h"
#include "event_manager.h"
#include <cmath>
#include <iostream>

EnemyTower::EnemyTower(int x, int y)
    : position_x(x), position_y(y), health(6), max_health(6), attack_range(4), attack_cooldown(3), current_cooldown(0), damage(1), can_attack_this_turn(true), alive(true) {}

void EnemyTower::update(Player& player, const Playing_field& field) {
    if (!alive) return;
    
    can_attack_this_turn = true;
    
    if (current_cooldown > 0) {
        current_cooldown--;
        can_attack_this_turn = false;
        return;
    }
    
    if (!player.get_alive()) {
        return;
    }
    
    int distance_x = std::abs(position_x - player.get_position_x());
    int distance_y = std::abs(position_y - player.get_position_y());
    
    if (distance_x <= attack_range && distance_y <= attack_range) {
        if (attack(player, field)) {
            current_cooldown = attack_cooldown;
            can_attack_this_turn = false;
            std::cout << "Enemy tower goes on cooldown for " << attack_cooldown << " turns." << std::endl;
        }
    }
}

bool EnemyTower::attack(Player& player, const Playing_field& field) {
    if (!can_attack_this_turn || !alive) {
        return false;
    }
    
    int player_x = player.get_position_x();
    int player_y = player.get_position_y();
    
    int distance_x = std::abs(position_x - player_x);
    int distance_y = std::abs(position_y - player_y);
    
    if (distance_x > attack_range || distance_y > attack_range) {
        return false;
    }
    
    bool has_line_of_sight = true;
    
    if (position_x == player_x) {
        int start_y = std::min(position_y, player_y);
        int end_y = std::max(position_y, player_y);
        for (int y = start_y + 1; y < end_y; y++) {
            if (!field.get_cell(position_x, y).can_pass()) {
                has_line_of_sight = false;
                break;
            }
        }
    } else if (position_y == player_y) {
        int start_x = std::min(position_x, player_x);
        int end_x = std::max(position_x, player_x);
        for (int x = start_x + 1; x < end_x; x++) {
            if (!field.get_cell(x, position_y).can_pass()) {
                has_line_of_sight = false;
                break;
            }
        }
    } else {
        has_line_of_sight = false;
    }
    
    if (has_line_of_sight && player.get_alive()) {
        std::cout << "Enemy tower shoots you for " << damage << " damage!" << std::endl;
        EventManager::trigger(EventType::DamageDealt, "Enemy Tower", damage);
        player.taking_damage(damage);
        return true;
    }
    
    return false;
}

void EnemyTower::take_damage(int damage) {
    if (!alive) return;
    
    health -= damage;
    EventManager::trigger(EventType::DamageTaken, "Enemy Tower", damage, health);
    std::cout << "Tower takes " << damage << " damage! Health: " << health << "/" << max_health << std::endl;
    
    if (health <= 0) {
        health = 0;
        alive = false;
        EventManager::trigger(EventType::UnitDied, "Enemy Tower destroyed");
        std::cout << ">>> ENEMY TOWER DESTROYED! <<<" << std::endl; 
    }
}

void EnemyTower::set_health(int new_health) {
    if (new_health < 0) {
        health = 0;
        alive = false;
    } else {
        health = std::min(new_health, max_health);
        alive = (health > 0);
    }
}

void EnemyTower::set_max_health(int new_max_health) {
    if (new_max_health <= 0) {
        throw std::invalid_argument("Max health must be positive");
    }
    max_health = new_max_health;
    if (health > max_health) {
        health = max_health;
    }
}

void EnemyTower::set_alive(bool new_alive) {
    alive = new_alive;
    if (!alive && health > 0) {
        health = 0;
    }
}

int EnemyTower::get_position_x() const { return position_x; }
int EnemyTower::get_position_y() const { return position_y; }
bool EnemyTower::get_can_attack() const { return can_attack_this_turn; }
int EnemyTower::get_attack_range() const { return attack_range; }
int EnemyTower::get_health() const { return health; }
int EnemyTower::get_max_health() const { return max_health; }
bool EnemyTower::is_alive() const { return alive; } 