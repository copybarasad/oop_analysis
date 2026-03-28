#include "level_manager.h"
#include "entity_manager.h"
#include "hand.h"
#include <random>
#include <algorithm>
#include <iostream>

std::unique_ptr<GameState> LevelManager::create_level(int level_number) {
    int width = calculate_level_width(level_number);
    int height = calculate_level_height(level_number);
    int enemy_count = calculate_enemy_count(level_number);
    
    auto game_state = std::make_unique<GameState>(width, height);
    
    EntityManager entity_manager(game_state->getField(), game_state->getPlayer(), game_state->getEnemies());
    entity_manager.spawnEnemies(enemy_count);
    
    return game_state;
}

void LevelManager::apply_level_modifiers(GameState& game_state, int level_number) {
    int enemy_health_bonus = calculate_enemy_health(level_number) - 2; 
    int enemy_damage_bonus = calculate_enemy_damage(level_number) - 1; 
    
    for (auto& enemy : game_state.getEnemies()) {
        enemy.increase_health(enemy_health_bonus);
        enemy.increase_damage(enemy_damage_bonus);
    }
    
    int tower_count = std::min(level_number, 3);
    game_state.spawn_towers(tower_count);
    
    std::cout << "Level " << level_number << " modifiers applied. " << "Enemy health bonus: +" << enemy_health_bonus<< ", damage bonus: +" << enemy_damage_bonus << std::endl;
}

void LevelManager::upgrade_player(Player& player, int upgrade_choice) {
    switch (upgrade_choice) {
        case 1: 
            player.set_max_life(player.get_max_life() + 1);
            player.set_current_life(player.get_max_life()); 
            std::cout << "Max health increased to " << player.get_max_life() << std::endl;
            break;
            
        case 2: 
            player.set_melee_damage(player.get_melee_damage() + 1);
            std::cout << "Melee damage increased to " << player.get_melee_damage() << std::endl;
            break;
            
        case 3: 
            player.set_ranged_damage(player.get_ranged_damage() + 1);
            std::cout << "Ranged damage increased to " << player.get_ranged_damage() << std::endl;
            break;
            
        case 4: 
            player.increase_spell_slots(1);
            std::cout << "Spell slots increased" << std::endl;
            break;
            
        default:
            std::cout << "Invalid upgrade choice! No upgrade applied." << std::endl;
            break;
    }
}

int LevelManager::calculate_level_width(int level) {
    return BASE_WIDTH + (level - 1) * 2;
}

int LevelManager::calculate_level_height(int level) {
    return BASE_HEIGHT + (level - 1) * 2;
}

int LevelManager::calculate_enemy_count(int level) {
    return BASE_ENEMY_COUNT + (level - 1);
}

int LevelManager::calculate_enemy_health(int level) {
    return 2 + (level / 2); 
}

int LevelManager::calculate_enemy_damage(int level) {
    return 1 + (level / 3); 
}

void LevelManager::remove_half_spells(Player& player) {
    Hand& hand = player.get_spell_hand();
    int spell_count = hand.get_spell_count();
    
    if (spell_count > 0) {
        int spells_to_remove = spell_count / 2;
        std::vector<int> indices(spell_count);
        
        for (int i = 0; i < spell_count; ++i) {
            indices[i] = i;
        }
        
        std::random_device rd;
        std::mt19937 generator(rd());
        std::shuffle(indices.begin(), indices.end(), generator);
        
        std::sort(indices.rbegin(), indices.rbegin() + spells_to_remove);
        
        for (int i = 0; i < spells_to_remove; ++i) {
            if (indices[i] < hand.get_spell_count()) {
                hand.remove_spell(indices[i]);
            }
        }
        
        std::cout << "Removed " << spells_to_remove << " spells. Ready for next level!" << std::endl;
    } else {
        std::cout << "No spells to remove. Ready for next level!" << std::endl;
    }
}