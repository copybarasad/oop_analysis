#include "level_manager.h"
#include <algorithm>

LevelManager::LevelManager() 
    : current_level(1), max_level_width(25), max_level_height(25) {}

void LevelManager::next_level() {
    current_level++;
}

int LevelManager::calculate_level_width(int base_width) const {
    int new_width = base_width + 1;
    if (new_width > max_level_width) {
        new_width = 10;
    }
    return new_width;
}

int LevelManager::calculate_level_height(int base_height) const {
    int new_height = base_height + 1;
    if (new_height > max_level_height) {
        new_height = 10;
    }
    return new_height;
}

int LevelManager::calculate_enemy_health(int base_health) const {
    return base_health + (current_level - 1) * 10;
}

int LevelManager::calculate_enemy_damage(int base_damage) const {
    return base_damage + (current_level - 1) * 2;
}