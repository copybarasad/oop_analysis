#include "game_controller.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <algorithm>

bool Game_controller::has_accessible_neighbors(const Position& pos) const {
    auto neighbors = game_field.get_adjacent_positions(pos);
    return !neighbors.empty();
}

Position Game_controller::find_position_with_escape(int max_attempts) const {
    int width = game_field.get_width();
    int height = game_field.get_height();
    
    for (int attempt = 0; attempt < max_attempts; ++attempt) {
        Position pos(rand() % width, rand() % height);
        if (game_field.is_position_accessible(pos) && has_accessible_neighbors(pos)) {
            return pos;
        }
    }
    
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            Position pos(x, y);
            if (game_field.is_position_accessible(pos)) {
                return pos;
            }
        }
    }
    
    return Position(0, 0);
}

Game_controller::Game_controller(int field_height, int field_width, int enemy_count)
    : game_field(field_width, field_height), game_over(false), save_system(), 
      level_manager(), base_width(field_width), base_height(field_height), 
      base_enemy_count(enemy_count),
      current_width(field_width), current_height(field_height), current_enemy_count(enemy_count)
{
    std::srand(std::time(nullptr));

    Position player_start = find_position_with_escape(100);
    
    player = std::make_unique<Player>(100, 20, 15, player_start, 3);
    
    spawn_enemies(base_enemy_count);
}

void Game_controller::set_level_parameters(int width, int height, int enemy_count) {
    current_width = width;
    current_height = height;
    current_enemy_count = enemy_count;
}

void Game_controller::spawn_enemies(int enemy_count) {
    for (int i = 0; i < enemy_count; ++i) {
        Position enemy_pos;
        
        do {
            enemy_pos = Position(rand() % game_field.get_width(), rand() % game_field.get_height());
        } while (!game_field.is_position_accessible(enemy_pos) || 
                 enemy_pos == player->get_position() ||
                 !is_position_free_for_enemy(enemy_pos));
        
        int enemy_health = level_manager.calculate_enemy_health(50);
        int enemy_damage = level_manager.calculate_enemy_damage(10);
        enemies.push_back(std::make_unique<Enemy>(enemy_health, enemy_damage, enemy_pos));
    }
}

bool Game_controller::is_position_free_for_enemy(const Position& pos) const {
    for (const auto& enemy : enemies) {
        if (enemy->get_position() == pos) return false;
    }
    return true;
}

bool Game_controller::has_obstacle_between(const Position& start, const Position& end) const {
    if (start.get_x() == end.get_x()) {
        int min_y = std::min(start.get_y(), end.get_y());
        int max_y = std::max(start.get_y(), end.get_y());
        for (int y = min_y + 1; y < max_y; y++) {
            Position check_pos(start.get_x(), y);
            if (game_field.get_cell_type(check_pos) == Cell_type::OBSTACLE) {
                return true;
            }
        }
    } else if (start.get_y() == end.get_y()) {
        int min_x = std::min(start.get_x(), end.get_x());
        int max_x = std::max(start.get_x(), end.get_x());
        for (int x = min_x + 1; x < max_x; x++) {
            Position check_pos(x, start.get_y());
            if (game_field.get_cell_type(check_pos) == Cell_type::OBSTACLE) {
                return true;
            }
        }
    }
    return false;
}

void Game_controller::process_player_move(const Position& new_position) {
    if (!game_field.is_position_accessible(new_position)) {
        std::cout << "Cannot move there! Cell is blocked.\n";
        return;
    }

    for (auto it = enemies.begin(); it != enemies.end(); ) {
        if ((*it)->get_position() == new_position) {
            (*it)->take_damage(player->get_damage());
            std::cout << "You attack enemy for " << player->get_damage() << " damage!\n";
            
            if (!(*it)->is_alive()) {
                player->on_enemy_defeated();
                std::cout << "Enemy defeated! +10 points.\n";
                it = enemies.erase(it);
            } else {
                player->take_damage((*it)->get_damage());
                std::cout << "Enemy counterattacks for " << (*it)->get_damage() << " damage!\n";
                ++it;
            }
            return;
        } else {
            ++it;
        }
    }
    
    player->set_position(new_position);
}

void Game_controller::process_player_combat_style_switch(Combat_style new_style) {
    player->switch_combat_style(new_style);
    std::cout << "Switched to " << (new_style == Combat_style::CLOSE ? "CLOSE" : "RANGED") << " combat.\n";
}

void Game_controller::process_player_ranged_attack() {
    if (player->get_combat_style() != Combat_style::RANGED) {
        std::cout << "You need to be in RANGED combat style to use ranged attack!\n";
        return;
    }

    Position player_pos = player->get_position();
    bool attacked = false;
    
    for (auto it = enemies.begin(); it != enemies.end(); ) {
        Position enemy_pos = (*it)->get_position();
        int dx = std::abs(player_pos.get_x() - enemy_pos.get_x());
        int dy = std::abs(player_pos.get_y() - enemy_pos.get_y());
        
        if (dx <= 2 && dy <= 2 && (dx > 0 || dy > 0)) {
            if (!has_obstacle_between(player_pos, enemy_pos)) {
                (*it)->take_damage(player->get_damage());
                attacked = true;
                std::cout << "Ranged attack hits enemy for " << player->get_damage() << " damage!\n";
                
                if (!(*it)->is_alive()) {
                    player->on_enemy_defeated();
                    std::cout << "Enemy defeated! +10 points.\n";
                    it = enemies.erase(it);
                } else {
                    ++it;
                }
            } else {
                std::cout << "Attack blocked by obstacle!\n";
                ++it;
            }
        } else {
            ++it;
        }
    }
    
    if (!attacked) {
        std::cout << "No enemies in range or all attacks blocked by obstacles!\n";
    }
}

void Game_controller::process_player_spell_cast(int spell_index, const Position& target) {
    player->cast_spell(spell_index, *this, target);
}

void Game_controller::process_player_get_spell() {
    player->try_get_new_spell();
}

void Game_controller::advance_to_next_level() {
    if (!can_advance_level()) return;
    
    std::cout << "=== ADVANCING TO NEXT LEVEL ===\n";
    
    current_width = game_field.get_width();
    current_height = game_field.get_height();
    
    player->level_up();
    
    level_manager.next_level();
    initialize_new_level();
}

void Game_controller::initialize_new_level() {
    int new_width = level_manager.calculate_level_width(current_width);
    int new_height = level_manager.calculate_level_height(current_height);
    
    int enemy_count = current_enemy_count;
    
    std::cout << "New field size: " << new_width << "x" << new_height << "\n";
    std::cout << "Enemies: " << enemy_count << "\n";
    
    current_width = new_width;
    current_height = new_height;
    current_enemy_count = enemy_count;
    
    game_field = Gamefield(new_width, new_height);
    
    player->heal(player->get_max_health());
    std::cout << "Health restored to maximum: " << player->get_max_health() << " HP\n";
    
    remove_half_spells();
    
    spawn_enemies(enemy_count);
    
    Position player_start = find_position_with_escape(100);
    player->set_position(player_start);
    
    std::cout << "Level " << level_manager.get_current_level() << " ready!\n";
}

void Game_controller::remove_half_spells() {
    auto& spell_hand = player->get_spell_hand();
    int current_spells = spell_hand.get_current_size();
    
    if (current_spells <= 1) {
        std::cout << "Spells unchanged: " << current_spells << " spell(s) remaining\n";
        return;
    }
    
    int spells_to_remove;
    if (current_spells % 2 == 0) {
        spells_to_remove = current_spells / 2;
    } else {
        spells_to_remove = (current_spells - 1) / 2;
    }
    
    std::cout << "Removing " << spells_to_remove << " out of " << current_spells << " spells\n";
    
    for (int i = 0; i < spells_to_remove; i++) {
        if (spell_hand.get_current_size() > 0) {
            spell_hand.remove_spell(spell_hand.get_current_size() - 1);
        }
    }
    
    std::cout << spell_hand.get_current_size() << " spells remaining\n";
}

void Game_controller::move_enemies() {
    for (auto& enemy : enemies) {
        auto adjacent = game_field.get_adjacent_positions(enemy->get_position());
        if (!adjacent.empty()) {
            Position new_pos = adjacent[rand() % adjacent.size()];
            if (new_pos == player->get_position()) {
                player->take_damage(enemy->get_damage());
                std::cout << "Enemy attacks you for " << enemy->get_damage() << " damage!\n";
            } else {
                enemy->set_position(new_pos);
            }
        }
    }
}

void Game_controller::process_enemy_turn() {
    move_enemies();
}

void Game_controller::load_player_state(int health, int max_health, int damage, const Position& position, 
                                      int score, Combat_style style) {
    if (player) {
        player->set_position(position);
        player->switch_combat_style(style);
        int damage_taken = max_health - health;
        if (damage_taken > 0) {
            player->take_damage(damage_taken);
        }
        for (int i = 0; i < score / 10; i++) {
            player->add_score(10);
        }
    }
}

void Game_controller::load_enemy_state(int health, int max_health, int damage, const Position& position) {
    auto enemy = std::make_unique<Enemy>(max_health, damage, position);
    int damage_taken = max_health - health;
    if (damage_taken > 0) {
        enemy->take_damage(damage_taken);
    }
    enemies.push_back(std::move(enemy));
    current_enemy_count = enemies.size();
}

void Game_controller::clear_enemies() {
    enemies.clear();
}

void Game_controller::load_gamefield_state(int width, int height, const std::vector<std::string>& map_data) {
    game_field.create_from_data(width, height, map_data);
    current_width = width;
    current_height = height;
    std::cout << "Gamefield restored: " << width << "x" << height << std::endl;
}

void Game_controller::save_game() {
    save_system.save_game(*this);
}

bool Game_controller::load_game() {
    try {
        save_system.load_game(*this);
        return true;
    } catch (const std::exception& e) {
        std::cout << "Load failed: " << e.what() << std::endl;
        return false;
    }
}

bool Game_controller::can_load_game() const {
    return save_system.save_exists();
}

const Player& Game_controller::get_player() const {
    return *player;
}

Player& Game_controller::get_player() {
    return *player;
}

const Gamefield& Game_controller::get_game_field() const {
    return game_field;
}

Gamefield& Game_controller::get_game_field() {
    return game_field;
}

const std::vector<std::unique_ptr<Enemy>>& Game_controller::get_enemies() const {
    return enemies;
}

std::vector<std::unique_ptr<Enemy>>& Game_controller::get_enemies() {
    return enemies;
}

bool Game_controller::is_game_over() const {
    return game_over;
}