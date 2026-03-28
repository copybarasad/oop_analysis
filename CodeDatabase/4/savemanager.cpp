#include "savemanager.hpp"
#include "gameengine.hpp"
#include "exceptions.hpp"
#include "attacktower.hpp"
#include "fireball.hpp"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <memory>
#include <filesystem>

const std::string SaveManager::SAVE_DIR = "saves/";

void SaveManager::save_to_file(const GameEngine& game_state, const std::string& filename) {
    std::string full_path = SAVE_DIR + filename;
    std::ofstream file(full_path, std::ios::binary);
    if (!file.is_open()) {
        throw FileIOException("Could not open file for writing: " + full_path);
    }
    try {
        write_int(file, game_state.get_current_level());
        
        const Hero& player = game_state.get_player();
        write_int(file, player.health);
        write_int(file, player.damage);
        write_int(file, player.get_mana());
        write_int(file, player.upgrade_level);
        write_int(file, player.skip_turn ? 1 : 0);
        write_int(file, player.max_health);
        write_int(file, static_cast<int>(player.weapon));
        write_int(file, player.sword_damage);
        write_int(file, player.bow_damage);
        write_int(file, static_cast<int>(player.atack_distance));
        
        write_int(file, static_cast<int>(player.handle.size()));
        for (size_t i = 0; i < player.handle.size(); ++i) {
            auto& spell_ptr = player.handle.get_spell_at_index(i);
            if (spell_ptr) {
                int spell_id = spell_ptr->get_spell_id();
                int upgrade_count = spell_ptr->get_upgrade_count();
                write_int(file, spell_id);
                write_int(file, upgrade_count);
            } else {
                write_int(file, -1);
                write_int(file, 0);
            }
        }

        if (game_state.current_level_instance) {
            const Level& level = game_state.current_level_instance.value();
            write_int(file, level.number);
            
            write_int(file, level.field.width);
            write_int(file, level.field.height);
            write_int(file, level.field.x);
            write_int(file, level.field.y);
            
            for (const auto& row : level.field.box) {
                for (const auto& cell : row) {
                    write_int(file, static_cast<int>(cell.get_type()));
                    write_int(file, static_cast<int>(cell.get_entity()));
                    write_int(file, cell.id);
                }
            }

            write_int(file, level.roster.level);
            write_int(file, level.roster.enemy_id_counter);
            write_int(file, level.roster.ally_id_counter);

            write_int(file, static_cast<int>(level.roster.enemies.size()));
            for (const auto& enemy_pair : level.roster.enemies) {
                const Enemy& e = enemy_pair.first;
                const auto& pos = enemy_pair.second;
                write_int(file, e.id);
                write_int(file, e.health);
                write_int(file, e.damage);
                write_int(file, e.skip_turn ? 1 : 0);
                write_int(file, pos.first);
                write_int(file, pos.second);
            }

            write_int(file, static_cast<int>(level.roster.towers.size()));
            for (const auto& tower_pair : level.roster.towers) {
                const Tower& t = tower_pair.first;
                const auto& pos = tower_pair.second;
                write_int(file, t.id);
                write_int(file, t.health);
                write_int(file, t.damage);
                write_int(file, t.current_timer);
                write_int(file, t.cooldown);
                write_int(file, static_cast<int>(t.level));
                write_int(file, pos.first);
                write_int(file, pos.second);
            }

            write_int(file, static_cast<int>(level.roster.allies.size()));
            for (const auto& ally_pair : level.roster.allies) {
                const Ally& a = ally_pair.first;
                const auto& pos = ally_pair.second;
                write_int(file, a.id);
                write_int(file, a.health);
                write_int(file, a.damage);
                write_int(file, a.skip_turn ? 1 : 0);
                write_int(file, pos.first);
                write_int(file, pos.second);
            }

            write_int(file, static_cast<int>(level.roster.attack_towers.size()));
            for (const auto& at_pair : level.roster.attack_towers) {
                const AttackTower& at = at_pair.first;
                const auto& pos = at_pair.second;
                write_int(file, at.id);
                write_int(file, at.health);
                write_int(file, at.damage);
                write_int(file, at.cooldown);
                write_int(file, at.current_cooldown);
                

                int fireball_upgrades = at.fireball_spell.get_upgrade_count();
                write_int(file, fireball_upgrades);
                
                write_int(file, pos.first);
                write_int(file, pos.second);
            }
        } else {
            write_int(file, -1);
        }

        file.close();
    } catch (...) {
        file.close();
        std::filesystem::remove(full_path);
        throw;
    }
}

GameEngine SaveManager::load_from_file(const std::string& filename) {
    std::string full_path = SAVE_DIR + filename;
    std::ifstream file(full_path, std::ios::binary);
    if (!file.is_open()) {
        throw FileIOException("Could not open file for reading: " + full_path);
    }
    try {
        GameEngine game_state;
        
        int loaded_level_num = read_int(file);
        game_state.set_current_level(loaded_level_num);
        
        Hero loaded_hero;
        loaded_hero.health = read_int(file);
        loaded_hero.damage = read_int(file);
        loaded_hero.mana = read_int(file);
        loaded_hero.upgrade_level = read_int(file);
        loaded_hero.skip_turn = read_int(file) != 0;
        loaded_hero.max_health = read_int(file);
        loaded_hero.weapon = static_cast<Weapon>(read_int(file));
        loaded_hero.sword_damage = read_int(file);
        loaded_hero.bow_damage = read_int(file);
        loaded_hero.atack_distance = static_cast<Distance>(read_int(file));
        loaded_hero.damage = loaded_hero.get_current_damage();
        
        int hand_size = read_int(file);
        loaded_hero.handle = Hand(6);
        for (int i = 0; i < hand_size; ++i) {
            int spell_id = read_int(file);
            int upgrade_count = read_int(file);
            if (spell_id != -1) {
                auto spell = loaded_hero.create_spell_by_id(spell_id);
                if (spell) {
                    for (int j = 0; j < upgrade_count; ++j) {
                        spell->upgrade();
                    }
                    loaded_hero.handle.add_spell(std::move(spell));
                }
            }
        }

        int saved_level_num = read_int(file);
        if (saved_level_num != -1) {
            int width = read_int(file);
            int height = read_int(file);
            int hero_x = read_int(file);
            int hero_y = read_int(file);
 
            EntityManager temp(0, 0, width, height);
            Field loaded_field(temp, width, height);
            loaded_field.x = hero_x;
            loaded_field.y = hero_y;
            
            for (int i = 0; i < height; ++i) {
                for (int j = 0; j < width; ++j) {
                    Type t = static_cast<Type>(read_int(file));
                    EntityType e = static_cast<EntityType>(read_int(file));
                    int id = read_int(file);
                    loaded_field.box[i][j] = FieldCell(t, e, id);
                }
            }

            EntityManager loaded_roster(0, 0);
            loaded_roster.level = read_int(file);
            loaded_roster.enemy_id_counter = read_int(file);
            loaded_roster.ally_id_counter = read_int(file);

            int num_enemies = read_int(file);
            for (int i = 0; i < num_enemies; ++i) {
                int id = read_int(file);
                int health = read_int(file);
                int damage = read_int(file);
                int skip_turn = read_int(file);
                int x = read_int(file);
                int y = read_int(file);
                
                Enemy enemy(loaded_roster.level, id);
                enemy.health = health;
                enemy.damage = damage;
                enemy.skip_turn = skip_turn != 0;
                
                loaded_roster.enemies.push_back(std::make_pair(std::move(enemy), std::make_pair(x, y)));
            }

            int num_towers = read_int(file);
            for (int i = 0; i < num_towers; ++i) {
                int id = read_int(file);
                int health = read_int(file);
                int damage = read_int(file);
                int current_timer = read_int(file);
                int cooldown = read_int(file);
                int level_val = read_int(file);
                int x = read_int(file);
                int y = read_int(file);
                
                Tower tower(cooldown, level_val, id);
                tower.health = health;
                tower.damage = damage;
                tower.current_timer = current_timer;
                tower.cooldown = cooldown;
                tower.level = level_val;
                
                loaded_roster.towers.push_back(std::make_pair(std::move(tower), std::make_pair(x, y)));
            }

            int num_allies = read_int(file);
            for (int i = 0; i < num_allies; ++i) {
                int id = read_int(file);
                int health = read_int(file);
                int damage = read_int(file);
                int skip_turn = read_int(file);
                int x = read_int(file);
                int y = read_int(file);
                
                Ally ally(id);
                ally.health = health;
                ally.damage = damage;
                ally.skip_turn = skip_turn != 0;
                
                loaded_roster.allies.push_back(std::make_pair(std::move(ally), std::make_pair(x, y)));
            }

            int num_attack_towers = read_int(file);
            for (int i = 0; i < num_attack_towers; ++i) {
                int id = read_int(file);
                int health = read_int(file);
                int damage = read_int(file);
                int cooldown = read_int(file);
                int current_cooldown = read_int(file);
                int fireball_upgrades = read_int(file);
                int x = read_int(file);
                int y = read_int(file);
                
                AttackTower at(id);
                at.health = health;
                at.damage = damage;
                at.cooldown = cooldown;
                at.current_cooldown = current_cooldown;

                for (int j = 0; j < fireball_upgrades; ++j) {
                    at.fireball_spell.upgrade();
                }
                
                loaded_roster.attack_towers.push_back(std::make_pair(std::move(at), std::make_pair(x, y)));
            }
            
            Level loaded_level(saved_level_num, 0, width, height);
            loaded_level.number = saved_level_num;
            loaded_level.field = std::move(loaded_field);
            loaded_level.roster = std::move(loaded_roster);
            
            game_state.current_level_instance = std::move(loaded_level);
        } else {
            int initial_enemy_count = 5;
            int initial_width = 15;
            int initial_height = 15;
            game_state.current_level_instance.emplace(loaded_level_num, initial_enemy_count, initial_width, initial_height);
        }

        game_state.set_player(std::move(loaded_hero));
        file.close();
        return game_state;
    } catch (...) {
        file.close();
        throw InvalidDataException("Failed to read or parse data from file: " + full_path);
    }
}

std::vector<std::string> SaveManager::list_save_files(const std::string& directory) {
    std::vector<std::string> saves;
    if (std::filesystem::exists(SAVE_DIR)) {
        for (const auto& entry : std::filesystem::directory_iterator(SAVE_DIR)) {
            if (entry.is_regular_file() && entry.path().extension() == ".sav") {
                saves.push_back(entry.path().filename().string());
            }
        }
    }
    return saves;
}

void SaveManager::write_int(std::ostream& out, int val) {
    out.write(reinterpret_cast<const char*>(&val), sizeof(val));
}

void SaveManager::write_string(std::ostream& out, const std::string& str) {
    size_t len = str.size();
    write_int(out, static_cast<int>(len));
    out.write(str.data(), len);
}

int SaveManager::read_int(std::istream& in) {
    int val;
    in.read(reinterpret_cast<char*>(&val), sizeof(val));
    return val;
}

std::string SaveManager::read_string(std::istream& in) {
    int len = read_int(in);
    std::string str(len, '\0');
    in.read(str.data(), len);
    return str;
}