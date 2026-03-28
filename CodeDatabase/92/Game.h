#pragma once

#include <string>
#include <vector>
#include "Player.h"
#include "Enemy.h"
#include "Field.h"
#include "EnemyTower.h"
#include "GameObserver.h"
#include "nlohmann/json.hpp"

#define COINS_FOR_KILL 100

using json = nlohmann::json;
class GameManager;

class Game {
public:
    Game(Player& player, std::vector<Enemy>& enemies, Field& field, std::vector<EnemyTower>& towers, int level, GameManager* manager);

    void set_observer(GameObserver* obs);
    void log(const std::string& msg);
    void refresh_observer();

    bool move_player(int dx, int dy);
    void toggle_weapon();
    bool cast_spell(int spell_idx, int x, int y);
    void buy_item(int shop_idx);
    void save_game_request(std::string filename);
    
    void update_ai();

    Field& get_field() { return field; }
    Player& get_player() { return player; }
    int get_player_spell_count();
    void print_spells_info(); 
    void print_shop_info();   

    bool place_trap(std::pair<int, int> position, float damage);
    
    bool is_finished_state() const { return is_game_finished; }
    bool is_victory_state() const { return victory; }
    int get_level() { return current_level; }

    json to_json();
    bool from_json(json& j);

    template<typename AttackerType>
    bool attack_cell(AttackerType &attacker, std::pair<int, int> position);

private:
    Player& player;
    std::vector<Enemy>& enemies;
    Field& field;
    std::vector<EnemyTower>& towers;
    GameManager* manager;
    GameObserver* observer = nullptr;

    bool is_game_finished = false;
    bool victory = false;
    int current_level = 1;
    std::vector<std::pair<std::pair<int, int>, float>> active_traps;

    int enemy_id_by_pos(std::pair<int, int> coordinates);
    int trap_id_by_pos(std::pair<int, int> coordinates);
    int tower_id_by_pos(std::pair<int, int> coordinates);

    void enemies_turn();
    void towers_turn();
    void trigger_trap(Entity& defender, std::pair<int, int> position);

    template<typename AttackerType>
    void attack(AttackerType& attacker, Entity& defender);
};

#include <format>
template<typename AttackerType>
bool Game::attack_cell(AttackerType &attacker, std::pair<int, int> position) {
    switch(field.getCellType(position)) {
        case Field::Cell::Player: 
            attack(attacker, player);
            return true;
        case Field::Cell::Enemy: {
            int enemy_id = enemy_id_by_pos(position);
            if (enemy_id != -1) {
                attack(attacker, enemies[enemy_id]);
                return true;
            }
            break;
        }
        case Field::Cell::Tower: {
            int tower_id = tower_id_by_pos(position);
            if (tower_id != -1) {
                attack(attacker, towers[tower_id]);
                return true;
            }
            break;
        }
        default: break;
    }
    return false;
}

template<typename AttackerType>
void Game::attack(AttackerType& attacker, Entity& defender){
    float damage = attacker.attack();
    defender.changeHealth(-damage);

    if (&defender != &player) {
        if (defender.getHealth() <= 0.0) {
            field.setCellType(defender.getPosition(), Field::Cell::Empty);
            int enemy_id = enemy_id_by_pos(defender.getPosition());
            if (enemy_id != -1) {
                enemies.erase(enemies.begin() + enemy_id);
                log("Враг убит!");
                player.add_points(1);
                player.add_coins(COINS_FOR_KILL);
            }

            int tower_id = tower_id_by_pos(defender.getPosition());
            if (tower_id != -1) {
                towers.erase(towers.begin() + tower_id);
                log("Башня разрушена!");
                player.add_points(2);
                player.add_coins(COINS_FOR_KILL * 2);
            }

            if (enemies.empty() && towers.empty()) {
                is_game_finished = true;
                victory = true;
                log("Все враги и башни уничтожены. Вы победили!");
            }
        } else {
             log("Вы нанесли " + std::to_string((int)damage) + " урона. У цели осталось " +
             std::to_string((int)defender.getHealth()));
        }
    } else {
        log("Вы получили " + std::to_string((int)damage) + " урона.");
        if (player.getHealth() <= 0.0f) {
            is_game_finished = true;
            log("Вы погибли. Игра окончена.");
        }
    }
}
