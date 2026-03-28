#include "GameField.h"
#include <iostream>
#include <cmath>
#include <cstdlib>

void Tower::update(GameField& game_field) {
    if(is_alive()){
        spawn_enemy(game_field);
        reduce_cooldown_spawn();
        
        attack(game_field);
        reduce_cooldown_attack();
    }
}

bool Tower::attack(GameField& game_field){
    if(!can_attack()) return false;

    int dx = game_field.get_player()->get_x() - get_x();
    int dy = game_field.get_player()->get_y() - get_y();
    
    double distance = std::sqrt(dx*dx + dy*dy);

    if(distance > sqrt(get_range_attack()*get_range_attack() + 1)){
        return false;
    }

    spell_.cast_by_tower(*game_field.get_player(), damage_multiplier_);

    reset_cooldown_attack();

    return true;
}

bool Tower::spawn_enemy(GameField& game_field){
    if (!can_spawn()) return false;
    
    int attempts = 20;
    while(attempts > 0) {
        attempts--;
        
        int dx = rand()%(range_spawn_ * 2 + 1) - range_spawn_;
        int dy = rand()%(range_spawn_ * 2 + 1) - range_spawn_;
        
        int x = get_x() + dx;
        int y = get_y() + dy;
        
        double distance = sqrt(dx*dx + dy*dy);
        if(distance <= sqrt(pow(range_spawn_, 2) + 1) && 
           game_field.is_cell_passable(x, y)){
            
            auto enemy = std::make_unique<Enemy>(health_spawned_enemy_, x, y);
            game_field.place_entity(std::move(enemy), x, y);
            reset_cooldown_spawn();
            return true;
        }
    }
    return false;
}

void Tower::show_stats() const {
    std::cout << "=== TOWER STATS ===" << std::endl;
    Entity::show_stats();
    std::cout << "Spawn Range: " << range_spawn_ 
              << "\nCooldown: " << cooldown_spawn_ << "/" << max_cooldown_spawn_ << std::endl;
}

std::string Tower::serialize() const{
    std::string data;

    data += Entity::serialize();
    data += "RANGE_SPAWN " + std::to_string(range_spawn_) + "\n";
    data += "COOLDOWN_SPAWN " + std::to_string(cooldown_spawn_) + " " + std::to_string(max_cooldown_spawn_) + "\n";
    data += "COOLDOWN_ATTACK " + std::to_string(cooldown_attack_) + " " + std::to_string(max_cooldown_attack_) + "\n";
    data += "SPELL " + spell_.get_name() + '\n';
    data += "HEALTH_SE " + std::to_string(health_spawned_enemy_) + "\n";
    data += "MULTIPLIER " + std::to_string(damage_multiplier_) + "\n";

    return data;
}