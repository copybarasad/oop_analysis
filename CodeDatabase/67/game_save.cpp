#include "game_save.h"
#include "game.h"
#include "player.h"
#include "enemy.h"
#include "enemy_tower.h"
#include "game_field.h"
#include "lightning_spell.h"
#include "fireball_spell.h"
#include "trap_spell.h"
#include <iostream>
#include <sstream>
#include <tuple>
#include <vector>


std::string GameSave::trim(const std::string& str) const {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, last - first + 1);
}

std::string GameSave::getValue(const std::string& line) const {
    size_t pos = line.find(':');
    if (pos == std::string::npos) {
        throw SaveGameException("Invalid save format: missing colon in line: " + line);
    }
    return trim(line.substr(pos + 1));
}

void GameSave::saveGame(const game* game_ptr, const std::string& filename) const {
    if (game_ptr == nullptr) {
        throw SaveGameException("Cannot save: game pointer is nullptr");
    }

    std::ofstream file(filename);
    if (!file.is_open()) {
        throw SaveGameException("Cannot open file for saving: " + filename);
    }

    try {
        file << "=== GAME SAVE ===" << std::endl;
        file << "Turn: " << game_ptr->get_turn() << std::endl;
        file << "Level: " << game_ptr->current_level << std::endl;
        file << std::endl;

        file << "=== PLAYER ===" << std::endl;
        Player* player = game_ptr->player_ptr;
        file << "HP: " << player->get_health_points() << std::endl;
        file << "X: " << player->get_position_x() << std::endl;
        file << "Y: " << player->get_position_y() << std::endl;
        file << "Kills: " << player->get_enemies_killed() << std::endl;

        Hand* hand = player->get_hand();
        file << "Spells: " << hand->getSpellCount() << std::endl;
        for (int i = 0; i < hand->getSpellCount(); i++) {
            Spell* spell = hand->getSpell(i);
            file << "  " << spell->getName() << " " 
                 << spell->getDamage() << " " 
                 << spell->getRadius() << std::endl;
        }
        file << std::endl;


        file << "=== ENEMIES ===" << std::endl;
        file << "Count: " << game_ptr->enemies.size() << std::endl;
        for (size_t i = 0; i < game_ptr->enemies.size(); i++) {
            Enemy* enemy = game_ptr->enemies[i];
            if (enemy != nullptr && !enemy->is_dead()) {
                file << "Enemy " << (i + 1) << std::endl;
                file << "  HP: " << enemy->get_health_points() << std::endl;
                file << "  X: " << enemy->get_position_x() << std::endl;
                file << "  Y: " << enemy->get_position_y() << std::endl;
                file << "  Damage: " << enemy->get_damage() << std::endl;
            }
        }
        file << std::endl;

        file << "=== TOWERS ===" << std::endl;
        file << "Count: " << game_ptr->towers.size() << std::endl;
        for (size_t i = 0; i < game_ptr->towers.size(); i++) {
            EnemyTower* tower = game_ptr->towers[i];
            if (tower != nullptr && !tower->is_destroyed()) {
                file << "Tower " << (i + 1) << std::endl;
                file << "  HP: " << tower->get_health() << std::endl;
                file << "  X: " << tower->getPositionX() << std::endl;
                file << "  Y: " << tower->getPositionY() << std::endl;
                file << "  Damage: " << tower->getDamage() << std::endl;
                file << "  Radius: " << tower->getRadius() << std::endl;
                file << "  Cooldown: " << tower->canAttack() << std::endl;
            }
        }
        file << std::endl;

        file << "=== FIELD ===" << std::endl;
        file << "Width: " << game_ptr->field_ptr->get_width() << std::endl;
        file << "Height: " << game_ptr->field_ptr->get_height() << std::endl;
        file << "Traps:" << std::endl;
        for (int y = 0; y < game_ptr->field_ptr->get_height(); y++) {
            for (int x = 0; x < game_ptr->field_ptr->get_width(); x++) {
                if (game_ptr->field_ptr->get_cell(x, y).hasTrap()) {
                    file << "  " << x << " " << y << " " 
                         << game_ptr->field_ptr->get_cell(x, y).getTrapDamage() << std::endl;
                }
            }
        }
        file << std::endl;

        file << "=== END ===" << std::endl;
        file.close();

        std::cout << "Game saved successfully to: " << filename << std::endl;

    } catch (const std::exception& e) {
        file.close();
        throw SaveGameException(std::string("Error during saving: ") + e.what());
    }
}

void GameSave::loadGame(game* game_ptr, const std::string& filename) {
    if (game_ptr == nullptr) {
        throw SaveGameException("Cannot load: game pointer is nullptr");
    }

    std::ifstream file(filename);
    if (!file.is_open()) {
        throw SaveGameException("Cannot open file for loading: " + filename);
    }

    try {
        std::string line;
        int player_hp = 0, player_x = 0, player_y = 0, player_kills = 0;
        std::vector<std::tuple<std::string, int, int>> spells_to_load;
        std::vector<std::tuple<int, int, int>> enemies_to_load;
        std::vector<std::tuple<int, int, int>> towers_to_load;
        std::vector<std::tuple<int, int, int>> traps_to_load;
        
        bool in_player_section = false;
        bool in_enemies_section = false;
        bool in_towers_section = false;
        bool in_field_section = false;
        
        int current_enemy_hp = 0, current_enemy_x = 0, current_enemy_y = 0;
        int current_tower_hp = 0, current_tower_x = 0, current_tower_y = 0;

        while (std::getline(file, line)) {
            line = trim(line);

            if (line.empty()) continue;

            if (line.find("=== PLAYER ===") != std::string::npos) {
                in_player_section = true;
                in_enemies_section = false;
                in_towers_section = false;
                in_field_section = false;
                continue;
            }

            else if (line.find("=== ENEMIES ===") != std::string::npos) {
                in_player_section = false;
                in_enemies_section = true;
                in_towers_section = false;
                in_field_section = false;
                continue;
            }

            else if (line.find("=== TOWERS ===") != std::string::npos) {
                in_player_section = false;
                in_enemies_section = false;
                in_towers_section = true;
                in_field_section = false;
                continue;
            }

            else if (line.find("=== FIELD ===") != std::string::npos) {
                in_player_section = false;
                in_enemies_section = false;
                in_towers_section = false;
                in_field_section = true;
                continue;
            }

            else if (line.find("Level:") != std::string::npos) {
                game_ptr->current_level = std::stoi(getValue(line));
                continue;
            }

            else if (line.find("=== END ===") != std::string::npos) {
                break;
            }

            if (in_player_section) {
                if (line.find("HP:") != std::string::npos) {
                    player_hp = std::stoi(getValue(line));
                }
                else if (line.find("X:") != std::string::npos) {
                    player_x = std::stoi(getValue(line));
                }
                else if (line.find("Y:") != std::string::npos) {
                    player_y = std::stoi(getValue(line));
                }
                else if (line.find("Kills:") != std::string::npos) {
                    player_kills = std::stoi(getValue(line));
                }
                else if (line.find("Spells:") == std::string::npos && line[0] != ' ' && line.find(" ") != std::string::npos) {
                    std::istringstream iss(line);
                    std::string spell_name;
                    int damage, radius;
                    if (iss >> spell_name >> damage >> radius) {
                        spells_to_load.push_back(std::make_tuple(spell_name, damage, radius));
                    }
                }
            }
            else if (in_enemies_section) {
                if (line.find("Enemy") != std::string::npos && line[0] != ' ') {
                    if (current_enemy_hp != 0) {
                        enemies_to_load.push_back(std::make_tuple(current_enemy_hp, current_enemy_x, current_enemy_y));
                    }
                    current_enemy_hp = 0;
                    current_enemy_x = 0;
                    current_enemy_y = 0;
                }
                else if (line.find("HP:") != std::string::npos) {
                    current_enemy_hp = std::stoi(getValue(line));
                }
                else if (line.find("X:") != std::string::npos) {
                    current_enemy_x = std::stoi(getValue(line));
                }
                else if (line.find("Y:") != std::string::npos) {
                    current_enemy_y = std::stoi(getValue(line));
                }
            }
            else if (in_towers_section) {
                if (line.find("Tower") != std::string::npos && line[0] != ' ') {
                    if (current_tower_hp != 0) {
                        towers_to_load.push_back(std::make_tuple(current_tower_hp, current_tower_x, current_tower_y));
                    }
                    current_tower_hp = 0;
                    current_tower_x = 0;
                    current_tower_y = 0;
                }
                else if (line.find("HP:") != std::string::npos) {
                    current_tower_hp = std::stoi(getValue(line));
                }
                else if (line.find("X:") != std::string::npos) {
                    current_tower_x = std::stoi(getValue(line));
                }
                else if (line.find("Y:") != std::string::npos) {
                    current_tower_y = std::stoi(getValue(line));
                }
            }
            else if (in_field_section) {
                if (line.find("Traps:") != std::string::npos) {
                    continue;
                }
                std::istringstream iss(line);
                int x, y, damage;
                if (iss >> x >> y >> damage) {
                    traps_to_load.push_back(std::make_tuple(x, y, damage));
                }
            }
        }

        if (current_enemy_hp != 0) {
            enemies_to_load.push_back(std::make_tuple(current_enemy_hp, current_enemy_x, current_enemy_y));
        }
        if (current_tower_hp != 0) {
            towers_to_load.push_back(std::make_tuple(current_tower_hp, current_tower_x, current_tower_y));
        }

        file.close();

        game_ptr->cleanup();

        int new_size = 10 + (game_ptr->current_level - 1);

        game_ptr->field_ptr = new game_field(new_size, new_size);
        game_ptr->player_ptr = new Player();

        Hand* hand = game_ptr->player_ptr->get_hand();
        hand->clearSpells();

        game_ptr->player_ptr->set_position(player_x, player_y, game_ptr->field_ptr);
        game_ptr->player_ptr->set_health(player_hp);
        game_ptr->field_ptr->get_cell(player_x, player_y).setPlayer(true);

        for (int i = 0; i < player_kills; i++) {
            game_ptr->player_ptr->increment_kills();
        }

        for (const auto& spell_data : spells_to_load) {
            std::string spell_name = std::get<0>(spell_data);
            int damage = std::get<1>(spell_data);
            int radius = std::get<2>(spell_data);
            
            Spell* spell = nullptr;
            if (spell_name == "Lightning") {
                spell = new LightningSpell(damage, radius);
            } else if (spell_name == "Fireball") {
                spell = new FireballSpell();
            } else if (spell_name == "Trap") {
                spell = new TrapSpell();
            }
            
            if (spell != nullptr) {
                hand->addSpell(spell);
            }
        }

        for (const auto& enemy_data : enemies_to_load) {
            int hp = std::get<0>(enemy_data);
            int x = std::get<1>(enemy_data);
            int y = std::get<2>(enemy_data);
            
            game_ptr->spawnEnemyAt(x, y);
            
            if (!game_ptr->enemies.empty()) {
                Enemy* last_enemy = game_ptr->enemies.back();
                last_enemy->set_health(hp);
            }
        }

        for (const auto& tower_data : towers_to_load) {
            int hp = std::get<0>(tower_data);
            int x = std::get<1>(tower_data);
            int y = std::get<2>(tower_data);
            
            game_ptr->spawnTowerAt(x, y);
            
            if (!game_ptr->towers.empty()) {
                EnemyTower* last_tower = game_ptr->towers.back();
                last_tower->take_damage(50 - hp);
            }
        }

        for (const auto& trap : traps_to_load) {
            int x = std::get<0>(trap);
            int y = std::get<1>(trap);
            int damage = std::get<2>(trap);
            if (game_ptr->field_ptr->isValidPosition(x, y)) {
                game_ptr->field_ptr->get_cell(x, y).setTrap(true, damage);
            }
        }

        std::cout << "Game loaded successfully from: " << filename << std::endl;

    } catch (const std::exception& e) {
        file.close();
        throw SaveGameException(std::string("Error during loading: ") + e.what());
    }
}
