#include "game_controller.hpp"
#include <random>
#include <algorithm>
#include <chrono>
#include "spell_direct_damage.hpp"
#include "spell_trap.hpp"
#include "save_and_load.hpp"

Game_controller::Game_controller(int field_height, int field_width, int enemy_count): game_field(field_height, field_width), game_over(false), current_level(1), 
    initial_enemy_count(enemy_count), level_completed(false){
        Position player_start_position(0, 0);
        player = std::make_unique<Player>(100, 70, 10, player_start_position, 2);
        spawn_enemies(initial_enemy_count, 10);
        spawn_towers(1);
}

bool Game_controller::is_position_free_for_enemy(const Position& pos) const{
    if(!game_field.is_position_accessible(pos)) return false;
    if(pos == player->get_position()) return false;
    for(auto& enemy: enemies){
        if(enemy->get_position() == pos) return false;
    }
    return true;
}

void Game_controller::attack_player_with_tower(Enemy_tower& tower){
    Position player_pos = player->get_position();
    int dx = std::abs(player_pos.get_x() - tower.get_position().get_x());
    int dy = std::abs(player_pos.get_y() - tower.get_position().get_y());
    bool was_attacked = false;
    if(dx <= tower.get_attack_radius() && dy <= tower.get_attack_radius()){
        if(game_field.is_line_clear(tower.get_position(), player_pos)) was_attacked = true;
    }
    tower.attack_player_if_in_range(*player, game_field);
    if(was_attacked){
        note_event(Damage_event("Tower(" + std::to_string(tower.get_position().get_x()) + "," + std::to_string(tower.get_position().get_y()) + ")", 
            "Player", tower.get_damage()));
    }
}

void Game_controller::spawn_enemies(int enemy_count, int enemy_damage){
    std::mt19937 gen(std::chrono::steady_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<int> x_distribution(0, game_field.get_width() - 1);
    std::uniform_int_distribution<int> y_distribution(0, game_field.get_height() - 1);
    int enemies_spawned = 0;
    while(enemies_spawned < enemy_count){
        int x = x_distribution(gen);
        int y = y_distribution(gen);
        Position enemy_position(x, y);
        if(is_position_free_for_enemy(enemy_position)){
            enemies.push_back(std::make_unique<Enemy>(100, enemy_damage, enemy_position));
            enemies_spawned++;
        };
    }
}

void Game_controller::process_player_move(const Position& new_position){
    if(game_over) return;
    Position old_pos = player->get_position();
    if(!game_field.is_position_accessible(new_position)){
        std::cout << "You can't move there - barricade!\n";
        return;
    }
    for(auto& trap: traps){
        if(trap.pos == new_position){
            std::cout << "You can't move there - trap!\n";
            return;
        }
    }
    for(auto& tower: towers){
        if(tower.get_position() == new_position){
            std::cout << "You can't move there - enemy tower!\n";
            return;
        }
    }
    Enemy* enemy_at_position = nullptr;
    for(auto& enemy: enemies){
        if(enemy->get_position() == new_position){
            enemy_at_position = enemy.get();
            break;
        }
    }
    if(enemy_at_position != nullptr){
        std::cout << "You attack enemy for " << player->get_damage() << " damage!\n";
        enemy_at_position->take_damage(player->get_damage());
        note_event(Damage_event("Player", "Enemy", player->get_damage()));
        if(!enemy_at_position->is_alive()){
            std::cout << "Enemy defeated! +10 points\n";
            player->add_score(10);
            for(auto enemy = enemies.begin(); enemy != enemies.end(); enemy++){
                if(enemy->get() == enemy_at_position){
                    enemies.erase(enemy);
                    break;
                }
            }
        }
    }
    else{
        player->set_position(new_position);
        note_event(Player_move_event(old_pos, new_position));
        std::cout << "Moved to position (" << new_position.get_x() << ", " << new_position.get_y() << ")\n";
    }
    process_enemy_turn();
}

void Game_controller::process_player_combat_style_switch(Combat_style new_style){
    if(game_over) return;
    player->switch_combat_style(new_style);
    process_enemy_turn();
}

void Game_controller::process_player_ranged_attack(){
    if(game_over) return;
    if(player->get_combat_style() != Combat_style::RANGED_COMBAT){
        std::cout << "You can use ranged attack only in ranged combat mode!\n";
        process_enemy_turn();
        return;
    }
    Position player_pos = player->get_position();
    std::vector<std::unique_ptr<Enemy>*> enemies_in_range;
    for(auto& enemy: enemies){
        if(!enemy->is_alive()) continue;
        Position enemy_pos = enemy->get_position();
        int dx = std::abs(enemy_pos.get_x() - player_pos.get_x());
        int dy = std::abs(enemy_pos.get_y() - player_pos.get_y());
        if(dx <= 2 && dy <= 2 && dx + dy > 0){
            if(game_field.is_line_clear(player_pos, enemy_pos)){
                enemies_in_range.push_back(&enemy);
            }
        }
    }
    if(enemies_in_range.empty()){
        std::cout << "No enemies in ranged attack radius (2 cells)\n";
        process_enemy_turn();
        return;
    }
    auto& target = *enemies_in_range[0];
    std::cout << "You shoot enemy at (" << target->get_position().get_x() << ", " 
            << target->get_position().get_y() << ") for " << player->get_damage() << " damage!\n";
    target->take_damage(player->get_damage());
    if (!target->is_alive()){
        std::cout << "Enemy defeated! +10 points\n";
        player->add_score(10);
        for(auto enemy = enemies.begin(); enemy != enemies.end(); enemy++){
            if(enemy->get() == target.get()){
                enemies.erase(enemy);
                break;
            }
        }
    }
    process_enemy_turn();
}

void Game_controller::process_enemy_turn(){
    if(game_over) return;
    move_enemies();
    for(auto& tower: towers){
        tower.update_cooldown();
        attack_player_with_tower(tower);
    }
    if(enemies.empty()){
        level_completed = true;
        return;
    }
    game_over = !player->is_alive();
}

void Game_controller::move_enemies(){
    for(auto& enemy: enemies){
        if(!enemy->is_alive()) continue;
        Position enemy_pos = enemy->get_position();
        Position player_pos = player->get_position();
        int dx = std::abs(enemy_pos.get_x() - player_pos.get_x());
        int dy = std::abs(enemy_pos.get_y() - player_pos.get_y());
        if(dx <= 1 && dy <= 1 && dx + dy == 1){
            player->take_damage(enemy->get_damage());
            note_event(Damage_event("Enemy", "Player", enemy->get_damage()));
            std::cout << "Enemy attacked you for " << enemy->get_damage() << " damage!\n";
        }
        else{
            auto adjacent = game_field.get_adjacent_positions(enemy_pos);
            if(!adjacent.empty()){
                static std::mt19937 gen(std::chrono::steady_clock::now().time_since_epoch().count());
                std::uniform_int_distribution<int> dist(0, adjacent.size() - 1);
                Position new_pos = adjacent[dist(gen)];
                bool occupied = false;
                for(auto& other: enemies){
                    if(other.get() != enemy.get() && other->get_position() == new_pos){
                        occupied = true;
                        break;
                    }
                }
                if(!occupied){
                    for(auto& tower: towers){
                        if(tower.get_position() == new_pos){
                            occupied = true;
                            break;
                        }
                    }
                }
                if(!occupied){
                    std::string enemy_id = "Enemy_" + std::to_string(&enemy - &enemies[0]);
                    note_event(Enemy_move_event(enemy_id, enemy_pos, new_pos));
                    enemy->set_position(new_pos);
                }
            }
        }
    }
    check_traps_on_enemies();
}

void Game_controller::process_player_spell_cast(int spell_index, Position target_pos){
    if(game_over) return;
    Player_hand& hand = player->get_hand();
    Position player_pos = player->get_position();
    Spell& spell = *hand.get_spells()[spell_index];
    if(!spell.is_valid_target(player_pos, target_pos)){
        std::cout << "Target is out of range!\n";
        process_enemy_turn();
        return;
    }
    if(dynamic_cast<Trap_spell*>(&spell)){
        place_trap(target_pos, spell.get_damage());
        auto spell_to_use = hand.use_spell(spell_index);
        process_enemy_turn();
        return;
    }
    if(dynamic_cast<Direct_damage_spell*>(&spell)){
        if(!game_field.is_line_clear(player_pos, target_pos)){
            std::cout << "Spell blocked by barricade!\n";
            process_enemy_turn();
            return;
        }
        bool enemy_found = false;
        for(auto& enemy: enemies){
            if(enemy->is_alive() && enemy->get_position() == target_pos){
                enemy_found = true;
                break;
            }
        }
        if(!enemy_found){
            std::cout << "No enemy at target position!\n";
            process_enemy_turn();
            return;
        }
    }
    std::vector<int> old_health;
    for(auto& enemy: enemies){
        old_health.push_back(enemy->is_alive() ? enemy->get_health() : -1);
    }
    auto spell_to_use = hand.use_spell(spell_index);
    spell_to_use->apply_effect(*player, target_pos, enemies, game_field);
    std::string spell_name = spell_to_use->get_name();
    for(int i = 0; i < enemies.size(); i++){
        if(old_health[i] >= 0 && enemies[i]->is_alive()){
            int damage_dealt = old_health[i] - enemies[i]->get_health();
            if(damage_dealt > 0){
                note_event(Spell_cast_event(spell_name, "Enemy_" + std::to_string(i), damage_dealt));
            }
        }
    }
    process_enemy_turn();
}

void Game_controller::process_player_buy_spell(){
    if(game_over) return;
    if(player->buy_spell(20)){
        std::cout << "You bought a new spell!\n";
        auto& hand = player->get_hand();
        note_event(Spell_add_event(hand.get_spells().back()->get_name()));
    }
    else{
        if(player->get_score() < 20){
            std::cout << "Not enough points!\n";
        }
        else{
            std::cout << "Your hand is full!\n";
        }
    }
    process_enemy_turn();
}

void Game_controller::place_trap(Position& pos, int damage){
    if(!game_field.is_position_accessible(pos)){
        std::cout << "Can't place trap on barricade!\n";
        return;
    }
    if(pos == player->get_position()){
        std::cout << "Can't place trap under player!\n";
        return;
    }
    for(auto& enemy: enemies){
        if(enemy->get_position() == pos){
            std::cout << "Can't place trap under enemy!\n";
            return;
        }
    }
    traps.push_back({pos, damage});
    std::cout << "Trap placed at (" << pos.get_x() << ", " << pos.get_y() << ")\n";
}

void Game_controller::check_traps_on_enemies(){
    auto trap = traps.begin();
    while(trap != traps.end()){
        bool triggered = false;
        for(auto& enemy : enemies){
            if(enemy->is_alive() && enemy->get_position() == trap->pos){
                int old_health = enemy->get_health();
                enemy->take_damage(trap->damage);
                int damage_dealt = old_health - enemy->get_health();
                note_event(Spell_cast_event("Trap", "Enemy", damage_dealt));
                std::cout << "Enemy stepped on trap! Took " << trap->damage << " damage!\n";
                trap = traps.erase(trap);
                triggered = true;
                break;
            }
        }
        if(!triggered) trap++;
    }
    for(auto enemy = enemies.begin(); enemy!= enemies.end();){
        if (!(*enemy)->is_alive()){
            enemy = enemies.erase(enemy);
        }
        else enemy++;
    }
}

void Game_controller::spawn_towers(int count){
    std::mt19937 gen(std::chrono::steady_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<int> x_dist(0, game_field.get_width() - 1);
    std::uniform_int_distribution<int> y_dist(0, game_field.get_height() - 1);
    int spawned = 0;
    int attempts = 0;
    while(spawned < count && attempts < 10){
        Position pos(x_dist(gen), y_dist(gen));
        if(game_field.is_position_accessible(pos) && pos != player->get_position() && is_position_free_for_enemy(pos)){
            bool tower_flag = false;
            for(auto& t: towers){
                if(t.get_position() == pos){
                    tower_flag = true;
                    break;
                }
            }
            if(tower_flag) continue;
            towers.push_back(Enemy_tower(pos, 10, 2));
            spawned++;
        }
        attempts++;
    }
}

void Game_controller::clear_all_entities(){
    enemies.clear();
    towers.clear();
    traps.clear();
}

void Game_controller::add_enemy(std::unique_ptr<Enemy> enemy){
    enemies.push_back(std::move(enemy));
}

void Game_controller::add_tower(const Enemy_tower& tower){
    towers.push_back(tower);
}

void Game_controller::add_trap(const Game_controller::Trap& trap){
    traps.push_back(trap);
}

void Game_controller::reset_player(int health, int close_damage, int ranged_damage, const Position& pos){
    player = std::make_unique<Player>(health, close_damage, ranged_damage, pos);
}

Game_controller::Game_controller(int field_height, int field_width, std::vector<std::vector<Cell_type>>&& field_grid)
    : game_field(field_height, field_width, field_grid), game_over(false), current_level(1), initial_enemy_count(3), level_completed(false){
    Position player_start(0, 0);
    player = std::make_unique<Player>(100, 25, 10, player_start, 2);
}

void Game_controller::load_from_file(const std::string& filename){
    Save_and_load loader;
    auto loaded = loader.load_game(filename);
    this->game_over = loaded->is_game_finished();
    this->player = std::move(loaded->player);
    this->enemies = std::move(loaded->enemies);
    this->towers = std::move(loaded->towers);
    this->traps = std::move(loaded->traps);
    this->game_field = std::move(const_cast<Gamefield&>(loaded->get_game_field()));
}

void Game_controller::proceed_to_next_level(){
    current_level++;
    int new_size = std::min(25, 10 + current_level - 1);
    game_field = Gamefield(new_size, new_size);
    player->set_position(Position(0, 0));
    player->heal(player->get_max_health());
    Player_hand& hand = player->get_hand();
    auto& spells = const_cast<std::vector<std::unique_ptr<Spell>>&>(hand.get_spells());
    int total = spells.size();
    if(total > 1){
        int to_remove = total / 2;
        std::mt19937 gen(std::chrono::steady_clock::now().time_since_epoch().count());
        for(int i = 0; i < to_remove; i++){
            std::uniform_int_distribution<size_t> dist(0, spells.size() - 1);
            int idx = dist(gen);
            spells.erase(spells.begin() + idx);
        }
    }
    clear_all_entities();
    int enemy_damage = 10 + (current_level - 1) * 5;
    spawn_enemies(initial_enemy_count, enemy_damage);
    spawn_towers(1);
    level_completed = false;
    game_over = false;
    std::cout << "\n>>> LEVEL " << current_level << " <<<\n";
    std::cout << "Field: " << new_size << "x" << new_size << "\n";
    std::cout << "Enemy damage: " << enemy_damage << "\n";
    std::cout << "Enemies: " << initial_enemy_count << "\n";
    std::cout << "Spells kept: " << hand.size() << "\n\n";
    on_level_complete(current_level);
}

void Game_controller::set_logging_enabled(Event_logger::Log_mode mode, const std::string& filename){
    logger.initialize(mode, filename);
}

void Game_controller::note_event(const Game_event& event) {
    logger.log_event(event);
}

void Game_controller::on_game_over(bool player_won) {
    note_event(Game_over_event(player_won));
}

void Game_controller::on_level_complete(int level) {
    note_event(Level_complete_event(level));
}