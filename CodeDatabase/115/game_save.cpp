#include "game_save.h"
#include "player.h"
#include "enemy.h"
#include "playing_field.h"
#include "enemy_tower.h"
#include "entity_manager.h"
#include <iostream>

void GameSave::save_game(const GameState& gameState, int level, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw GameSaveException("Cannot open file for writing: " + filename);
    }

    try {
        json game_data;
        game_data["version"] = 1;
        game_data["level"] = level;
        
        game_data["player"] = serialize_player(gameState.getPlayer());
        game_data["field"] = serialize_field(gameState.getField());
        game_data["enemies"] = serialize_enemies(gameState.getEnemies());
        game_data["towers"] = serialize_towers(gameState.getTowers());
        
        file << std::setw(2) << game_data << std::endl;
        
        if (!file.good()) {
            throw GameSaveException("Error occurred during file writing");
        }
        
    } catch (const std::exception& e) {
        throw GameSaveException("Save operation failed: " + std::string(e.what()));
    }
}

json GameSave::serialize_player(const Player& player) {
    json j;
    j["currentLife"] = player.get_curr_life();
    j["maxLife"] = player.get_max_life();
    j["meleeDamage"] = player.get_melee_damage();
    j["rangedDamage"] = player.get_ranged_damage();
    j["score"] = player.get_score();
    j["positionX"] = player.get_position_x();
    j["positionY"] = player.get_position_y();
    j["enhancementStacks"] = player.get_enhancement_stacks();
    j["maxSpellSlots"] = player.get_spell_hand().get_max_size();
    j["isAlive"] = player.get_alive();
    j["meleeMode"] = player.get_melee_mode();
    j["switching"] = player.get_switching();
    return j;
}

json GameSave::serialize_field(const Playing_field& field) {
    json j;
    int width = field.get_width();
    int length = field.get_length();
    
    j["width"] = width;
    j["length"] = length;
    
    json cells_array = json::array();
    for (int y = 0; y < length; ++y) {
        json row_array = json::array();
        for (int x = 0; x < width; ++x) {
            bool passable = field.get_cell(x, y).can_pass();
            row_array.push_back(passable);
        }
        cells_array.push_back(row_array);
    }
    j["cells"] = cells_array;
    
    return j;
}

json GameSave::serialize_enemies(const std::vector<Enemy>& enemies) {
    json enemies_array = json::array();
    
    for (const auto& enemy : enemies) {
        json enemy_json;
        enemy_json["currentLife"] = enemy.get_curr_life();
        enemy_json["maxLife"] = enemy.get_curr_life(); 
        enemy_json["damage"] = enemy.get_damage();
        enemy_json["positionX"] = enemy.get_position_x();
        enemy_json["positionY"] = enemy.get_position_y();
        enemy_json["isAlive"] = enemy.get_alive();
        enemies_array.push_back(enemy_json);
    }
    
    return enemies_array;
}

json GameSave::serialize_towers(const std::vector<EnemyTower>& towers) {
    json towers_array = json::array();
    
    for (const auto& tower : towers) {
        json tower_json;
        tower_json["positionX"] = tower.get_position_x();
        tower_json["positionY"] = tower.get_position_y();
        tower_json["health"] = tower.get_health();
        tower_json["maxHealth"] = tower.get_max_health();
        tower_json["isAlive"] = tower.is_alive();
        towers_array.push_back(tower_json);
    }
    
    return towers_array;
}

std::pair<std::unique_ptr<GameState>, int> GameSave::load_game(const std::string& filename) {
    std::ifstream file(filename);
    validate_save_file(file, filename);

    try {
        json game_data;
        file >> game_data;
        
        int level = game_data["level"];
        int width = game_data["field"]["width"];
        int length = game_data["field"]["length"];
        
        auto gameState = std::make_unique<GameState>(width - 2, length - 2, false);
        
        deserialize_player(game_data["player"], gameState->getPlayer());
        deserialize_field(game_data["field"], gameState->getField());
        deserialize_enemies(game_data["enemies"], gameState->getEnemies());
        deserialize_towers(game_data["towers"], gameState->getTowers());
        
        std::cout << "Game loaded successfully! Level: " << level << std::endl;
        std::cout << "Player position: " << gameState->getPlayer().get_position_x() << ", " << gameState->getPlayer().get_position_y() << std::endl;
        std::cout << "Enemies count: " << gameState->getEnemies().size() << std::endl;
        
        return {std::move(gameState), level};
        
    } catch (const std::exception& e) {
        throw GameSaveException("Load operation failed: " + std::string(e.what()));
    }
}

void GameSave::deserialize_player(const json& j, Player& player) {
    player.set_max_life(j["maxLife"]);
    player.set_current_life(j["currentLife"]);
    player.set_melee_damage(j["meleeDamage"]);
    player.set_ranged_damage(j["rangedDamage"]);
    
    player.set_score(j["score"]);
    
    player.reset_enhancement();
    if (j["enhancementStacks"] > 0) {
        player.add_enhancement(j["enhancementStacks"]);
    }
    
    int saved_max_slots = j["maxSpellSlots"];
    int current_max_slots = player.get_spell_hand().get_max_size();
    if (saved_max_slots != current_max_slots) {
        player.get_spell_hand().increase_max_size(saved_max_slots - current_max_slots);
    }
    
    int target_x = j["positionX"];
    int target_y = j["positionY"];
    player.set_position(target_x, target_y);
    
    player.set_alive(j["isAlive"]);
    player.set_melee_mode(j["meleeMode"]);
}

void GameSave::deserialize_field(const json& j, Playing_field& field) {
    int width = j["width"];
    int length = j["length"];
    const auto& cells = j["cells"];
    
    for (int y = 0; y < length; ++y) {
        for (int x = 0; x < width; ++x) {
            bool passable = cells[y][x];
            field.get_cell(x, y).set_empty(passable);
        }
    }
}

void GameSave::deserialize_enemies(const json& j, std::vector<Enemy>& enemies) {
    enemies.clear();
    
    for (const auto& enemy_json : j) {
        int x = enemy_json["positionX"];
        int y = enemy_json["positionY"];
        
        Enemy enemy(x, y);
        enemy.set_health(enemy_json["currentLife"]);
        enemy.set_damage(enemy_json["damage"]);
        enemy.set_alive(enemy_json["isAlive"]);
        
        enemies.push_back(enemy);
    }
}

void GameSave::deserialize_towers(const json& j, std::vector<EnemyTower>& towers) {
    towers.clear();
    
    for (const auto& tower_json : j) {
        int x = tower_json["positionX"];
        int y = tower_json["positionY"];
        
        EnemyTower tower(x, y);
        tower.set_health(tower_json["health"]);
        tower.set_max_health(tower_json["maxHealth"]);
        tower.set_alive(tower_json["isAlive"]);
        
        towers.push_back(tower);
    }
}

void GameSave::validate_save_file(std::ifstream& file, const std::string& filename) {
    if (!file.is_open()) {
        throw GameSaveException("Cannot open save file: " + filename);
    }

    file.seekg(0, std::ios::end);
    std::streamsize file_size = file.tellg();
    file.seekg(0, std::ios::beg);

    if (file_size == 0) {
        throw GameSaveException("Save file is empty: " + filename);
    }

    if (file_size < 10) {
        throw GameSaveException("Save file appears corrupted (too small): " + filename);
    }
}