#include "save_load_manager.h"
#include "game.h"
#include "player.h"
#include "game_field.h"
#include "hand.h"
#include "enemy.h"
#include "spell_card.h"
#include <fstream>
#include <filesystem>
#include <iostream>

Save_load_manager::FileHandler::FileHandler(const std::string& name, std::ios::openmode mode) 
    : file_name(name) {
    file.open(file_name, mode);
    if (!file.is_open()) {
        throw File_not_found_exception();
    }
}

Save_load_manager::FileHandler::~FileHandler() {
    if (file.is_open()) {
        file.close();
    }
}

void Save_load_manager::save_game(const Game& game, const std::string& file_name){
    FileHandler file(file_name, std::ios::binary | std::ios::out);

    try{
        int current_level = game.get_current_level();
        int total_turns = game.get_total_turns();
        file.get().write(reinterpret_cast<const char*>(&current_level), sizeof(current_level));
        file.get().write(reinterpret_cast<const char*>(&total_turns), sizeof(total_turns));
        
        int field_width = game.get_field()->get_width();
        int field_height = game.get_field()->get_height();
        file.get().write(reinterpret_cast<const char*>(&field_width), sizeof(field_width));
        file.get().write(reinterpret_cast<const char*>(&field_height), sizeof(field_height));
        
        save_player(file.get(), *game.get_player());
        save_field(file.get(), *game.get_field());
        if (!file.good()){
            throw Injured_file_exception();
        }
        std::cout << "Игра успешно сохранена в файл: " << file_name << std::endl;
    }
    catch (const std::exception& e){
        throw Injured_file_exception();
    }
}

Game Save_load_manager::load_game(const std::string& file_name){
    FileHandler file(file_name, std::ios::binary | std::ios::in);

    try{
        int current_level, total_turns;
        file.get().read(reinterpret_cast<char*>(&current_level), sizeof(current_level));
        file.get().read(reinterpret_cast<char*>(&total_turns), sizeof(total_turns));
        
        int field_width, field_height;
        file.get().read(reinterpret_cast<char*>(&field_width), sizeof(field_width));
        file.get().read(reinterpret_cast<char*>(&field_height), sizeof(field_height));

        Game game;
        game.initialize_objects(field_width, field_height);
        game.set_current_level(current_level);
        game.set_total_turns(total_turns);

        load_player(file.get(), *game.get_player());
        load_field(file.get(), *game.get_field());

        if (!file.good() && !file.get().eof()){
            throw Injured_file_exception();
        }
        std::cout << "Игра успешно загружена из файла: " << file_name << std::endl;
        std::cout << "Уровень: " << current_level << ", Ходов: " << total_turns << std::endl;
        return game;
    }
    catch (const std::exception& e){
        throw Injured_file_exception();
    }
}

void Save_load_manager::save_player(std::fstream& file, const Player& player){  
    try {
        std::string name = player.get_name();
        size_t name_length = name.length();
        file.write(reinterpret_cast<const char*>(&name_length), sizeof(name_length));
        file.write(name.c_str(), name_length);
        
        int health = player.get_health();
        int max_health = player.get_max_health();
        int melee_damage = player.get_melee_damage();
        int ranged_damage = player.get_ranged_damage();
        int score = player.get_score();
        int level = player.get_level();
        int mana = player.get_mana();
        int max_mana = player.get_max_mana();
        Combat_mode combat_mode = player.get_combat_mode();
        bool is_slowed = player.get_is_slowed();
        
        file.write(reinterpret_cast<const char*>(&health), sizeof(health));
        file.write(reinterpret_cast<const char*>(&max_health), sizeof(max_health));
        file.write(reinterpret_cast<const char*>(&melee_damage), sizeof(melee_damage));
        file.write(reinterpret_cast<const char*>(&ranged_damage), sizeof(ranged_damage));
        file.write(reinterpret_cast<const char*>(&score), sizeof(score));
        file.write(reinterpret_cast<const char*>(&level), sizeof(level));
        file.write(reinterpret_cast<const char*>(&mana), sizeof(mana));
        file.write(reinterpret_cast<const char*>(&max_mana), sizeof(max_mana));
        file.write(reinterpret_cast<const char*>(&combat_mode), sizeof(combat_mode));
        file.write(reinterpret_cast<const char*>(&is_slowed), sizeof(is_slowed));
        
        const auto& hand = player.get_spell_hand();
        size_t spell_count = hand.get_spell_count();
        file.write(reinterpret_cast<const char*>(&spell_count), sizeof(spell_count));        
    } 
    catch (const std::exception& e) {
        throw Injured_file_exception();
    }
}

void Save_load_manager::load_player(std::fstream& file, Player& player) {
    try {
        size_t name_length;
        file.read(reinterpret_cast<char*>(&name_length), sizeof(name_length));
        std::string name(name_length, ' ');
        file.read(&name[0], name_length);
        
        int health, max_health, melee_damage, ranged_damage, score, level, mana, max_mana;
        Combat_mode combat_mode;
        bool is_slowed;
        
        file.read(reinterpret_cast<char*>(&health), sizeof(health));
        file.read(reinterpret_cast<char*>(&max_health), sizeof(max_health));
        file.read(reinterpret_cast<char*>(&melee_damage), sizeof(melee_damage));
        file.read(reinterpret_cast<char*>(&ranged_damage), sizeof(ranged_damage));
        file.read(reinterpret_cast<char*>(&score), sizeof(score));
        file.read(reinterpret_cast<char*>(&level), sizeof(level));
        file.read(reinterpret_cast<char*>(&mana), sizeof(mana));
        file.read(reinterpret_cast<char*>(&max_mana), sizeof(max_mana));
        file.read(reinterpret_cast<char*>(&combat_mode), sizeof(combat_mode));
        file.read(reinterpret_cast<char*>(&is_slowed), sizeof(is_slowed));
        
        player.set_health(health);
        player.set_max_health(max_health);
        player.set_melee_damage(melee_damage);
        player.set_ranged_damage(ranged_damage);
        player.set_score(score);
        player.set_level(level);
        player.set_mana(mana);
        player.set_max_mana(max_mana);
        player.set_combat_mode(combat_mode);
        player.set_slowed(is_slowed);

        size_t spell_count;
        file.read(reinterpret_cast<char*>(&spell_count), sizeof(spell_count));
    } 
    catch (const std::exception& e) {
        throw Injured_file_exception();
    }
}

void Save_load_manager::save_field(std::fstream& file, const Game_field& field){  
    try {
        auto player_pos = field.get_player_position();
        file.write(reinterpret_cast<const char*>(&player_pos.first), sizeof(player_pos.first));
        file.write(reinterpret_cast<const char*>(&player_pos.second), sizeof(player_pos.second));
        
        int width = field.get_width();
        int height = field.get_height();
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                Cell cell = field.get_cell(x, y);
                Cell_type type = cell.get_type();
                file.write(reinterpret_cast<const char*>(&type), sizeof(type));
            }
        }
        
        auto enemy_positions = field.get_enemy_positions();
        size_t enemy_count = enemy_positions.size();
        file.write(reinterpret_cast<const char*>(&enemy_count), sizeof(enemy_count));
        
         for (size_t i = 0; i < enemy_count; ++i) {
            auto pos = enemy_positions[i];
            file.write(reinterpret_cast<const char*>(&pos.first), sizeof(pos.first));
            file.write(reinterpret_cast<const char*>(&pos.second), sizeof(pos.second));
            
            std::string enemy_name = "Враг";  
            size_t name_length = enemy_name.length();
            file.write(reinterpret_cast<const char*>(&name_length), sizeof(name_length));
            file.write(enemy_name.c_str(), name_length);
            
            int enemy_health = 50;  
            file.write(reinterpret_cast<const char*>(&enemy_health), sizeof(enemy_health));
        }

        size_t building_count = 0;
        file.write(reinterpret_cast<const char*>(&building_count), sizeof(building_count));
        
        size_t tower_count = 0;  
        file.write(reinterpret_cast<const char*>(&tower_count), sizeof(tower_count));
    } 
    catch (const std::exception& e) {
        throw Injured_file_exception();
    }
}

void Save_load_manager::load_field(std::fstream& file, Game_field& field) {
    try {
        int player_x, player_y;
        file.read(reinterpret_cast<char*>(&player_x), sizeof(player_x));
        file.read(reinterpret_cast<char*>(&player_y), sizeof(player_y));
        
        field.place_player(player_x, player_y);
        
        int width = field.get_width();
        int height = field.get_height();
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                Cell_type type;
                file.read(reinterpret_cast<char*>(&type), sizeof(type));
                field.set_cell_type(x, y, type);
            }
        }
        
        size_t enemy_count;
        file.read(reinterpret_cast<char*>(&enemy_count), sizeof(enemy_count));
        
        for (size_t i = 0; i < enemy_count; ++i) {
            int enemy_x, enemy_y;
            file.read(reinterpret_cast<char*>(&enemy_x), sizeof(enemy_x));
            file.read(reinterpret_cast<char*>(&enemy_y), sizeof(enemy_y));
            
            size_t name_length;
            file.read(reinterpret_cast<char*>(&name_length), sizeof(name_length));
            std::string enemy_name(name_length, ' ');
            file.read(&enemy_name[0], name_length);
            
            int enemy_health;
            file.read(reinterpret_cast<char*>(&enemy_health), sizeof(enemy_health));
            
            Enemy* enemy = new Enemy(enemy_name, &field);
            enemy->set_health(enemy_health);  
            field.add_enemy(enemy, enemy_x, enemy_y);
        }

        size_t building_count;
        file.read(reinterpret_cast<char*>(&building_count), sizeof(building_count));
        
        for (size_t i = 0; i < building_count; ++i) {
            int building_x, building_y;
            file.read(reinterpret_cast<char*>(&building_x), sizeof(building_x));
            file.read(reinterpret_cast<char*>(&building_y), sizeof(building_y));
            
            size_t name_length;
            file.read(reinterpret_cast<char*>(&name_length), sizeof(name_length));
            std::string building_name(name_length, ' ');
            file.read(&building_name[0], name_length);
        }
        
        size_t tower_count;
        file.read(reinterpret_cast<char*>(&tower_count), sizeof(tower_count));
        
        for (size_t i = 0; i < tower_count; ++i) {
            int tower_x, tower_y;
            file.read(reinterpret_cast<char*>(&tower_x), sizeof(tower_x));
            file.read(reinterpret_cast<char*>(&tower_y), sizeof(tower_y));
            
            size_t name_length;
            file.read(reinterpret_cast<char*>(&name_length), sizeof(name_length));
            std::string tower_name(name_length, ' ');
            file.read(&tower_name[0], name_length);
        }
    } 
    catch (const std::exception& e) {
        throw Injured_file_exception();
    }
}

bool Save_load_manager::save_exists(const std::string& file_name) {
    return std::filesystem::exists(file_name);
}