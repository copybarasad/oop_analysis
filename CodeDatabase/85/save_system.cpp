#include "save_system.h"
#include "game_controller.h"
#include "player.h"
#include "enemy.h"
#include "direct_damage_spell.h"
#include "area_damage_spell.h"
#include <iostream>
#include <sstream>
#include <memory>
#include <algorithm>
#include <vector>

SaveSystem::SaveSystem(const std::string& filename) 
    : save_filename(filename) {}

bool SaveSystem::save_exists() const {
    std::ifstream file(save_filename);
    return file.good();
}

void SaveSystem::save_game(const Game_controller& game) {
    std::ofstream file(save_filename);
    
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open save file for writing: " + save_filename);
    }
    
    try {
        const Player& player = game.get_player();
        const Gamefield& field = game.get_game_field();
        const auto& enemies = game.get_enemies();
        const auto& spells = player.get_spell_hand().get_spells();

        file << "SAVE_FILE_VERSION 1.3\n";
        
        file << "LEVEL " << game.get_current_level() << "\n";
        
        file << "LEVEL_PARAMS " 
             << game.get_current_width() << " " << game.get_current_height() << " "
             << game.get_current_enemy_count() << "\n";
        
        file << "PLAYER " 
             << player.get_health() << " " << player.get_max_health() << " "
             << player.get_damage() << " " << player.get_position().get_x() << " "
             << player.get_position().get_y() << " " << player.get_score() << " "
             << (player.get_combat_style() == Combat_style::CLOSE ? "CLOSE" : "RANGED") << "\n";
        
        file << "PLAYER_STATS " 
             << player.get_close_damage() << " " << player.get_ranged_damage() << "\n";
        
        file << "FIELD " << field.get_width() << " " << field.get_height() << "\n";
        
        file << "MAP\n";
        std::vector<std::string> map_data = field.get_map_data();
        for (const auto& row : map_data) {
            file << row << "\n";
        }
        file << "END_MAP\n";
        
        file << "ENEMIES " << enemies.size() << "\n";
        for (const auto& enemy : enemies) {
            file << "ENEMY "
                 << enemy->get_health() << " " << enemy->get_max_health() << " "
                 << enemy->get_damage() << " " << enemy->get_position().get_x() << " "
                 << enemy->get_position().get_y() << "\n";
        }
        
        file << "SPELLS " << spells.size() << "\n";
        for (const auto& spell : spells) {
            std::string name = spell->get_name();
            std::string desc = spell->get_description();
            std::replace(name.begin(), name.end(), ' ', '_');
            std::replace(desc.begin(), desc.end(), ' ', '_');
            
            file << "SPELL "
                 << spell->get_type() << " " << name << " " << desc << " "
                 << spell->get_range() << " ";
            
            if (spell->get_type() == "Direct Damage") {
                auto* direct_spell = dynamic_cast<DirectDamageSpell*>(spell.get());
                if (direct_spell) {
                    file << direct_spell->get_damage() << "\n";
                }
            } else if (spell->get_type() == "Area Damage") {
                auto* area_spell = dynamic_cast<AreaDamageSpell*>(spell.get());
                if (area_spell) {
                    file << area_spell->get_damage() << " " << area_spell->get_area_size() << "\n";
                }
            } else {
                file << "0\n";
            }
        }
        
        const SpellHand& spell_hand = player.get_spell_hand();
        file << "SPELL_PROGRESS "
             << spell_hand.get_current_size() << " " << spell_hand.get_max_size() << " "
             << spell_hand.get_enemies_defeated() << " " << spell_hand.get_enemies_for_new_spell() << "\n";
        
        file << "END_SAVE\n";
        
    } catch (const std::exception& e) {
        file.close();
        throw std::runtime_error("Error during save: " + std::string(e.what()));
    }
    
    file.close();
    std::cout << "Game saved to: " << save_filename << std::endl;
}

void SaveSystem::load_game(Game_controller& game) {
    std::ifstream file(save_filename);
    
    if (!file.is_open()) {
        throw std::runtime_error("Save file not found: " + save_filename);
    }
    
    try {
        std::string line;
        int saved_level = 1;
        int player_close_damage = 20;
        int player_ranged_damage = 15;
        int level_width = 0, level_height = 0, level_enemy_count = 0;
        
        std::getline(file, line);
        if (line.find("SAVE_FILE_VERSION") == std::string::npos) {
            throw std::runtime_error("Invalid save file format");
        }
        
        std::getline(file, line);
        std::istringstream level_stream(line);
        std::string level_tag;
        level_stream >> level_tag >> saved_level;
        
        if (level_tag != "LEVEL") {
            saved_level = 1;
            file.seekg(-static_cast<int>(line.length()) - 1, std::ios_base::cur);
        }
        
        std::getline(file, line);
        std::istringstream params_stream(line);
        std::string params_tag;
        params_stream >> params_tag;
        
        if (params_tag == "LEVEL_PARAMS") {
            params_stream >> level_width >> level_height >> level_enemy_count;
            std::cout << "Loaded level parameters: " << level_width << "x" 
                      << level_height << ", enemies: " << level_enemy_count << std::endl;
        } else {
            level_width = 10;
            level_height = 10;
            level_enemy_count = 3;
            file.seekg(-static_cast<int>(line.length()) - 1, std::ios_base::cur);
            std::cout << "Using default level parameters (legacy save file)" << std::endl;
        }
        
        game.set_level_parameters(level_width, level_height, level_enemy_count);
        
        std::getline(file, line);
        std::istringstream player_stream(line);
        std::string player_tag;
        int player_health, player_max_health, player_damage, player_x, player_y;
        int player_score;
        std::string combat_style_str;
        
        player_stream >> player_tag >> player_health >> player_max_health 
                     >> player_damage >> player_x >> player_y >> player_score
                     >> combat_style_str;
        
        if (player_tag != "PLAYER") {
            throw std::runtime_error("Invalid save file: player data missing");
        }
        
        Combat_style style = (combat_style_str == "CLOSE") ? Combat_style::CLOSE : Combat_style::RANGED;
        
        std::getline(file, line);
        std::istringstream stats_stream(line);
        std::string stats_tag;
        stats_stream >> stats_tag;
        
        if (stats_tag == "PLAYER_STATS") {
            stats_stream >> player_close_damage >> player_ranged_damage;
            std::cout << "Loaded player stats: Close=" << player_close_damage 
                      << ", Ranged=" << player_ranged_damage << std::endl;
        } else {
            file.seekg(-static_cast<int>(line.length()) - 1, std::ios_base::cur);
            std::cout << "Using default player stats (legacy save file)" << std::endl;
        }
        
        std::getline(file, line);
        std::istringstream field_stream(line);
        std::string field_tag;
        int field_width, field_height;
        field_stream >> field_tag >> field_width >> field_height;
        
        if (field_tag != "FIELD") {
            throw std::runtime_error("Invalid save file: field data missing");
        }
        
        std::getline(file, line);
        std::vector<std::string> map_data;
        while (std::getline(file, line) && line != "END_MAP") {
            map_data.push_back(line);
        }
        
        game.load_gamefield_state(field_width, field_height, map_data);
        
        game.clear_enemies();
        game.get_player().get_spell_hand().clear_spells();
        
        game.load_player_state(player_health, player_max_health, player_damage, 
                              Position(player_x, player_y), player_score, style);
        
        Player& player = game.get_player();
        
        int close_bonus = player_close_damage - 20;
        int ranged_bonus = player_ranged_damage - 15;
        
        if (close_bonus > 0) {
            player.upgrade_close_damage(close_bonus);
        }
        if (ranged_bonus > 0) {
            player.upgrade_ranged_damage(ranged_bonus);
        }
        
        game.set_level(saved_level);
        
        std::getline(file, line);
        std::istringstream enemies_stream(line);
        std::string enemies_tag;
        int enemy_count;
        enemies_stream >> enemies_tag >> enemy_count;
        
        if (enemies_tag != "ENEMIES") {
            throw std::runtime_error("Invalid save file: enemies data missing");
        }
        
        for (int i = 0; i < enemy_count; ++i) {
            std::getline(file, line);
            std::istringstream enemy_stream(line);
            std::string enemy_tag;
            int enemy_health, enemy_max_health, enemy_damage, enemy_x, enemy_y;
            
            enemy_stream >> enemy_tag >> enemy_health >> enemy_max_health 
                        >> enemy_damage >> enemy_x >> enemy_y;
            
            if (enemy_tag != "ENEMY") {
                throw std::runtime_error("Invalid enemy data in save file");
            }
            
            game.load_enemy_state(enemy_health, enemy_max_health, enemy_damage, 
                                 Position(enemy_x, enemy_y));
        }
        
        std::getline(file, line);
        std::istringstream spells_stream(line);
        std::string spells_tag;
        int spells_count;
        spells_stream >> spells_tag >> spells_count;
        
        if (spells_tag != "SPELLS") {
            throw std::runtime_error("Invalid save file: spells data missing");
        }
        
        auto& spell_hand = game.get_player().get_spell_hand();
        
        for (int i = 0; i < spells_count; ++i) {
            std::getline(file, line);
            std::istringstream spell_stream(line);
            std::string spell_tag, spell_type, spell_name, spell_desc;
            int spell_range;
            
            spell_stream >> spell_tag >> spell_type >> spell_name >> spell_desc 
                        >> spell_range;
            
            if (spell_tag != "SPELL") {
                throw std::runtime_error("Invalid spell data in save file");
            }
            
            for (char& c : spell_desc) {
                if (c == '_') c = ' ';
            }
            for (char& c : spell_name) {
                if (c == '_') c = ' ';
            }
            
            std::unique_ptr<Spell> spell;
            
            if (spell_type == "Direct Damage") {
                int damage;
                spell_stream >> damage;
                spell = std::make_unique<DirectDamageSpell>(spell_name, spell_desc, 
                                                           spell_range, damage);
            } else if (spell_type == "Area Damage") {
                int damage, area_size;
                spell_stream >> damage >> area_size;
                spell = std::make_unique<AreaDamageSpell>(spell_name, spell_desc, 
                                                         spell_range, damage, area_size);
            }
            
            if (spell) {
                spell_hand.add_spell(std::move(spell));
            }
        }
        
        std::getline(file, line);
        std::istringstream progress_stream(line);
        std::string progress_tag;
        int current_size, max_size, defeated, required;
        progress_stream >> progress_tag >> current_size >> max_size >> defeated >> required;
        
        if (progress_tag == "SPELL_PROGRESS") {
            spell_hand.set_enemies_defeated(defeated);
            spell_hand.set_enemies_for_new_spell(required);
            std::cout << "Spell progress loaded: " << defeated << "/" << required << " enemies\n";
        }
        
        std::getline(file, line);
        if (line != "END_SAVE") {
            std::cout << "Warning: Save file might be corrupted" << std::endl;
        }
        
        std::cout << "Game loaded successfully from: " << save_filename << std::endl;
        std::cout << "Level: " << saved_level << std::endl;
        std::cout << "Player: " << player_health << " HP, " << player_score << " score" << std::endl;
        std::cout << "Player Stats: Close=" << player_close_damage << ", Ranged=" << player_ranged_damage << std::endl;
        std::cout << "Enemies: " << enemy_count << ", Spells: " << spells_count << std::endl;
        std::cout << "Field: " << field_width << "x" << field_height << std::endl;
        
    } catch (const std::exception& e) {
        file.close();
        throw std::runtime_error("Error during load: " + std::string(e.what()));
    }
    
    file.close();
}

void SaveSystem::delete_save() {
    if (save_exists()) {
        if (std::remove(save_filename.c_str()) == 0) {
            std::cout << "Save file deleted: " << save_filename << std::endl;
        } else {
            throw std::runtime_error("Could not delete save file: " + save_filename);
        }
    }
}