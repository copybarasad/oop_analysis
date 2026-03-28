#include "Game.h"
#include <cmath>
#include "Pair_operations.h"
#include "TrapSpell.h"
#include "GameManager.h"

Game::Game(Player& player, std::vector<Enemy>& enemies, Field& field, std::vector<EnemyTower>& towers, int level, GameManager* manager) :
    player(player), enemies(enemies), field(field), towers(towers), current_level(level), manager(manager) {}

void Game::set_observer(GameObserver* obs) {
    this->observer = obs;
}

void Game::log(const std::string& msg) {
    if(observer) observer->on_log_message(msg);
}

void Game::refresh_observer() {
    if(observer) observer->on_field_change();
}

void Game::update_ai() {
    if (is_game_finished) return;
    enemies_turn();
    if (is_game_finished) return;
    towers_turn();
}

bool Game::move_player(int dx, int dy) {
    std::pair<int, int> target = player.getPosition() + std::make_pair(dx, dy);
    Field::Cell result = field.moveEntity(player, target);
    
    if (result == Field::Cell::Wall){
        log("Вы упёрлись в стену. Ход пропущен.");
        return true; 
    }

    if (result == Field::Cell::Trap) {
        log("Вы наступили на ловушку.");
        trigger_trap(player, target);
    }

    if (result == Field::Cell::Enemy) {
        int enemy_id = enemy_id_by_pos(target);
        if(enemy_id != -1) attack(player, enemies[enemy_id]);
    }
    else if (result == Field::Cell::Tower) {
        int tower_id = tower_id_by_pos(target);
        if(tower_id != -1) attack(player, towers[tower_id]);
    }

    if (player.is_far_mode()) {
        std::pair<int, int> offset = {dx, dy};
        std::pair<int, int> far_target = target;
        for (int i = 0; i < player.get_far_attack_reach(); i++) {
            far_target = far_target + offset;
            if (field.getCellType(far_target) == Field::Cell::Enemy) {
                int enemy_id = enemy_id_by_pos(far_target);
                if(enemy_id != -1) attack(player, enemies[enemy_id]);
            }
            else if (field.getCellType(far_target) == Field::Cell::Tower) {
                int tower_id = tower_id_by_pos(far_target);
                if(tower_id != -1) attack(player, towers[tower_id]);
            }
        }
    }
    return true;
}

void Game::toggle_weapon() {
    player.toggle_far_mode();
    log("Вы сменили оружие.");
}

void Game::save_game_request(std::string filename) {
    if (manager) manager->save_game(filename);
}

int Game::get_player_spell_count() {
    return player.get_spell_count();
}

void Game::print_spells_info() {
    if (observer) {
        observer->on_spells_display(player.get_player_spells_list());
    }
}

void Game::print_shop_info() {
    if (observer) {
        observer->on_shop_display(player.get_player_shop_list());
    }
}

bool Game::cast_spell(int spell_idx, int x, int y) {
    if (spell_idx < 0 || spell_idx >= player.get_spell_count()) return false;
    
    if (!field.inBounds({x, y})) {
        log("Эта координата за границами поля.");
        return false;
    }
    
    if (player.use_spell(spell_idx, *this, player.getPosition(), {x, y})) {
        log("Заклинание использовано.");
        return true;
    } else {
        log("Не удалось использовать заклинание (неверная цель или условия).");
        return false;
    }
}

void Game::buy_item(int shop_idx) {
    if (shop_idx < 0 || shop_idx >= player.get_shop_count()) {
        log("В магазине нет такого товара.");
        return;
    }
    if (player.get_spell_price(shop_idx) > player.get_coins()) {
        log("Недостаточно монет.");
        return;
    }
    player.spend_coins(player.get_spell_price(shop_idx));
    player.buy_spell(shop_idx);
    log("Заклинание приобретено!");
}

void Game::enemies_turn() {
    for (int i = enemies.size() - 1; i >= 0; i--) {
        if (is_game_finished) return;
        Enemy& enemy = enemies[i];

        int dx = player.getPosition().first - enemy.getPosition().first;
        int dy = player.getPosition().second - enemy.getPosition().second;
        std::pair<int, int> offset = {0,0};
        std::pair<int, int> secondary_offset = {0,0};
        
        if (std::abs(dx) > std::abs(dy)) {
            offset.first = (dx > 0) ? 1 : -1;
            if (dy != 0) secondary_offset.second = (dy > 0) ? 1 : -1;
        } else {
            offset.second = (dy > 0) ? 1 : -1;
            if (dx != 0) secondary_offset.first = (dx > 0) ? 1 : -1;
        }

        std::pair<int, int> primary_target = enemy.getPosition() + offset;
        std::pair<int, int> secondary_target = enemy.getPosition() + secondary_offset;

        if (primary_target == player.getPosition() || secondary_target == player.getPosition()) {
            attack(enemy, player);
            continue;
        }

        Field::Cell result = Field::Cell::Empty;
        if (field.isCellWalkable(primary_target)) {
            result = field.moveEntity(enemy, primary_target);
        } else if (field.isCellWalkable(secondary_target)) {
            result = field.moveEntity(enemy, secondary_target);
        }

        if (result == Field::Cell::Trap) {
            log("Враг наступил на ловушку.");
            trigger_trap(enemy, enemy.getPosition());
        }
    }
}

void Game::towers_turn() {
    for (auto &tower : towers) {
        tower.attack(*this, player.getPosition());
    }
}

int Game::enemy_id_by_pos(std::pair<int, int> coordinates) {
    for (int i = 0; i < enemies.size(); i++) {
        if (enemies[i].getPosition() == coordinates) return i;
    }
    return -1;
}

int Game::trap_id_by_pos(std::pair<int, int> coordinates) {
    for (int i = 0; i < active_traps.size(); i++) {
        if (active_traps[i].first == coordinates) return i;
    }
    return -1;
}

int Game::tower_id_by_pos(std::pair<int, int> coordinates) {
    for (int i = 0; i < towers.size(); i++) {
        if (towers[i].getPosition() == coordinates) return i;
    }
    return -1;
}

bool Game::place_trap(std::pair<int, int> position, float damage) {
    if (!field.isCellEmpty(position)) return false;

    active_traps.emplace_back(position, damage);
    field.setCellType(position, Field::Cell::Trap);
    return true;
}

void Game::trigger_trap(Entity& defender, std::pair<int, int> position) {
    int trap_id = trap_id_by_pos(position);
    if (trap_id == -1) return;
    
    int damage = active_traps[trap_id].second;
    TrapSpell single_use(damage, 0, Spell::Type::Trap);
    attack(single_use, defender);
    active_traps.erase(active_traps.begin() + trap_id);
}

json Game::to_json() {
    json j;
    j["level"] = current_level;
    j["traps"] = json::array();
    for(auto& t : active_traps) {
        j["traps"].push_back({
            {"x", t.first.first},
            {"y", t.first.second},
            {"dmg", t.second}
        });
    }
    return j;
}

bool Game::from_json(json& j) {
    active_traps.clear();
    try {
        if (j.contains("level")) current_level = j.at("level");
        else current_level = 1;
        
        if (j.contains("traps")) {
            for(auto& t : j.at("traps")) {
                int x = t.at("x");
                int y = t.at("y");
                float dmg = t.at("dmg");
                active_traps.emplace_back(std::make_pair(x, y), dmg);
            }
        }
        return true;
    } catch(...) { return false; }
}
