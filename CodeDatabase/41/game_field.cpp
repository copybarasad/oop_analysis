#include "game_field.h"
#include "player.h"
#include "enemy.h"
#include "building.h"
#include "enemy_tower.h"
#include "trap.h"
#include "ally.h" 
#include "constants.h" 
#include <iostream>
#include <random>
#include <algorithm>
#include <cmath>

template<typename T>
T clamp(const T& value, const T& low, const T& high) {
    return (value < low) ? low : (value > high) ? high : value;
}

Game_field::Game_field(int w, int h, Player* player_ptr)
    : width(clamp(w, 10, 25)), height(clamp(h, 10, 25)),
    player_x(-1), player_y(-1), player(player_ptr){ 
        initialize_field();
}

Game_field::Game_field(const Game_field& other) : player(nullptr){
    copy_from(other);
}

Game_field::Game_field(Game_field&& other) noexcept : player(nullptr){
    move_from(std::move(other));
}

Game_field& Game_field::operator=(const Game_field& other){
    if (this != &other){
        clear_field();
        copy_from(other);
    }
    return *this;
}

Game_field& Game_field::operator=(Game_field&& other) noexcept{
    if (this != &other){
        clear_field();
        move_from(std::move(other));
    }
    return *this;
}

Game_field::~Game_field(){
    clear_field();
}

int Game_field::get_height() const{
    return height;
}

int Game_field::get_width() const{
    return width;
}

Cell Game_field::get_cell(int x, int y) const{
    if (is_valid_position(x, y)){
        return cells[y][x];
    }
    return Cell(Cell_type::BLOCKED);
}

std::pair<int, int> Game_field::get_player_position() const{
    return {player_x, player_y};
}

std::vector<std::pair<int, int>> Game_field::get_enemy_positions() const{
    return enemy_positions;
}

void Game_field::initialize_field(){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis (0.0, 1.0);
    cells.resize(height);
    for (int y = 0; y < height; ++y){
        cells[y].resize(width);
        for (int x = 0; x < width; ++x){
            double rand_val = dis(gen);
            if (rand_val < 0.1){
                cells[y][x] = Cell(Cell_type::BLOCKED);
            } else if (rand_val < 0.2){
                cells[y][x] = Cell(Cell_type::SLOW);
            } else{
                cells[y][x] = Cell(Cell_type::EMPTY);
            }

            //новое
            cells[y][x].set_has_player(false);
            cells[y][x].set_has_enemy(false);
            cells[y][x].set_has_ally(false);    
            cells[y][x].set_has_building(false);
        }
    }
    cells[0][0] = Cell(Cell_type::EMPTY);
    cells[0][width - 1] = Cell(Cell_type::EMPTY);
    cells[height - 1][0] = Cell(Cell_type::EMPTY);
    cells[height -1][width - 1] = Cell(Cell_type::EMPTY);
}

bool Game_field::place_player(int x, int y){
    if (!is_valid_position(x, y) || !cells[y][x].can_move_to()){
        std::cout << "Нельзя разместить игрока в позиции (" << x << ", " << y << ")\n";
        return false;
    }
    if (player_x != -1 && player_y != -1){
        cells[player_y][player_x].set_has_player(false);
    }
    player_x = x;
    player_y = y;
    cells[y][x].set_has_player(true);
    return true;
}

void Game_field::add_enemy(Enemy* enemy, int x, int y){
    if (!is_valid_position(x, y) || !cells[y][x].can_move_to()){
        std::cout << "Нельзя разместить врага в позиции (" << x << ", " << y << ")\n";
        return;
    }
    enemies.push_back(std::unique_ptr<Enemy>(enemy));
    enemy_positions.push_back({x, y});
    cells[y][x].set_has_enemy(true);
}

void Game_field::add_building(const std::string& name, int x, int y, int spawn_interval){
    if (!is_valid_position(x, y) || !cells[y][x].is_empty()){
        std::cout << "Нельзя разместить здание в позиции (" << x << ", " << y << ")\n";
        return;
    }
    buildings.push_back(std::unique_ptr<Building>(new Building(name, x, y, this, spawn_interval)));
    cells[y][x].set_type(Cell_type::BUILDING);
    cells[y][x].set_has_building(true);
}

Move_status Game_field::move_player(int delta_x, int delta_y){
    if (!player){ 
        return Move_status::BLOCKED;
    }
    if (player->get_is_slowed()) {
        player->set_slowed(false);
        std::cout << "Эффект замедления прошел!\n";
        return Move_status::SUCCESS; 
    }

    int new_x = player_x + delta_x;
    int new_y = player_y + delta_y;

    if (!is_valid_position(new_x, new_y)){
        return Move_status::BLOCKED;
    }

    Cell& target_cell = cells[new_y][new_x];

    if (target_cell.get_has_enemy()){
        for (size_t i = 0; i < enemy_positions.size(); ++i){
            if (enemy_positions[i].first == new_x && enemy_positions[i].second == new_y){
                std::cout << "Игрок атакует врага!\n";
                perform_attack(player_x, player_y, new_x, new_y);
                return Move_status::ATTACKED;
            }
        }
    }
    if (!target_cell.can_move_to()){
        return Move_status::BLOCKED;
    }
    cells[player_y][player_x].set_has_player(false);
    player_x = new_x;
    player_y = new_y;
    cells[player_y][player_x].set_has_player(true);

    Move_status move_status = Move_status::SUCCESS;

    if (target_cell.get_type() == Cell_type::SLOW){
        player->set_slowed(true);
        std::cout << "Игрок замедлен!\n";
        move_status = Move_status::SLOWED;
    }
    return move_status;
}

bool Game_field::move_enemy(int enemy_index, int new_x, int new_y){
    if (enemy_index < 0 || enemy_index >= static_cast<int>(enemies.size()) || !is_valid_position(new_x, new_y)){
        return false;
    }
    if (cells[new_y][new_x].get_has_player()){
        perform_attack(enemy_positions[enemy_index].first, enemy_positions[enemy_index].second, new_x, new_y);
        return false;
    }
    if (!cells[new_y][new_x].can_move_to() || cells[new_y][new_x].get_has_enemy()){
        return false;
    }
    int old_x = enemy_positions[enemy_index].first;
    int old_y = enemy_positions[enemy_index].second;
    cells[old_y][old_x].set_has_enemy(false);
    cells[new_y][new_x].set_has_enemy(true);
    enemy_positions[enemy_index] = {new_x, new_y};
    return true;

}

void Game_field::update_enemies(){
    if (!player){
        return;
    }
    for (size_t i = 0; i < enemies.size(); ++i){
        if (enemies[i]->get_is_alive()) {
            enemies[i]->make_move(*player);
        }
    }
    for (size_t i = 0; i < enemies.size(); ) {
        if (!enemies[i]->get_is_alive()) {
            int dead_x = enemy_positions[i].first;
            int dead_y = enemy_positions[i].second;
            cells[dead_y][dead_x].set_has_enemy(false);
            enemies.erase(enemies.begin() + i);
            enemy_positions.erase(enemy_positions.begin() + i);
        } else {
            ++i;
        }
    }
}

bool Game_field::is_valid_position(int x, int y) const{
    return x >= 0 && y >= 0 && x < width && y < height;
}

bool Game_field::can_attack(int attacker_x, int attacker_y, int target_x, int target_y, Combat_mode mode) const{
    if (!is_valid_position(attacker_x, attacker_y) || !is_valid_position(target_x, target_y)){
        return false;
    }
    double distance = calculate_distance(attacker_x, attacker_y, target_x, target_y);
    if (mode == Combat_mode::MELEE){
        return distance <= 1.5;
    } else{
        return distance <= 3.5;
    }
}

void Game_field::perform_attack(int attacker_x, int attacker_y, int target_x, int target_y){
    if (attacker_x == player_x && attacker_y == player_y){
        for (size_t i = 0; i < enemy_positions.size(); ++i){
            if (enemy_positions[i].first == target_x && enemy_positions[i].second == target_y){
                if (enemies[i]->get_is_alive()){
                    int damage = player->get_damage();
                    enemies[i]->take_damage(damage);
                    std::cout << "Игрок атакует " << enemies[i]->get_name() << " и наносит " << damage << " урона!\n";
                    if (!enemies[i]->get_is_alive()){
                        cells[target_y][target_x].set_has_enemy(false);
                        player->increase_score(10);
                    }
                }
                break;
            }
        }
    } else{
        if (target_x == player_x && target_y == player_y){
            for (size_t i = 0; i < enemy_positions.size(); ++i){
                if (enemy_positions[i].first == attacker_x && enemy_positions[i].second == attacker_y){
                    if (enemies[i]->get_is_alive()){
                        int damage = enemies[i]->get_damage();
                        player->take_damage(damage);
                        std::cout << enemies[i]->get_name() << " атакует игрока и наносит " << damage << " урона!\n";
                    }
                    break;
                }
            }
        }
    }
}

void Game_field::update_buildings(){
    for (auto& building : buildings){
        building->update();
        if (building->should_spawn_enemy()){
            int bx = building->get_x();
            int by = building->get_y();
            std::vector<std::pair<int, int>> directions = {
                {0, 1}, {1, 0}, {0, -1}, {-1, 0},  
                {1, 1}, {1, -1}, {-1, 1}, {-1, -1} 
            };
            for (const auto& dir : directions){
                int spawn_x = bx + dir.first;
                int spawn_y = by + dir.second;
                if (is_valid_position(spawn_x, spawn_y) && cells[spawn_y][spawn_x].can_move_to()){
                    Enemy* new_enemy = new Enemy("Враг из " + building->get_name(), this);
                    add_enemy(new_enemy, spawn_x, spawn_y);
                    std::cout << building->get_name() << " создает врага в позиции (" << spawn_x << ", " << spawn_y << ")\n";
                    break;
                }
            }
        }
    }
    for (size_t i = 0; i < enemies.size(); ){
        if (!enemies[i]->get_is_alive()){
            int dead_x = enemy_positions[i].first;
            int dead_y = enemy_positions[i].second;
            cells[dead_y][dead_x].set_has_enemy(false);
            enemies.erase(enemies.begin() + i);
            enemy_positions.erase(enemy_positions.begin() + i);
        } else{
            ++i;
        }
    }

    for (size_t i = 0; i < buildings.size(); ){
        if (buildings[i]->is_destroyed()){
            destroy_building(i);
        } else{
            buildings[i]->update();
            ++i;
        }
    }

    update_traps();
}

std::pair<int, int> Game_field::get_enemy_position(const Enemy* enemy) const {
    for (size_t i = 0; i < enemies.size(); ++i) {
        if (enemies[i].get() == enemy) {
            return enemy_positions[i];
        }
    }
    return {-1, -1};
}

int Game_field::find_enemy_index(const Enemy* enemy) const {
    for (size_t i = 0; i < enemies.size(); ++i) {
        if (enemies[i].get() == enemy) {
            return i;
        }
    }
    return -1; 
}

bool Game_field::is_cell_blocked(int x, int y) const{
    return !is_valid_position(x, y) || !cells[y][x].is_passable();
}

double Game_field::calculate_distance(int x1, int y1, int x2, int y2) const{
    return std::sqrt(std::pow(x2 - x1, 2) + std::pow(y2 - y1, 2));
}

void Game_field::copy_from(const Game_field& other){
    width = other.width;
    height = other.height;
    player_x = other.player_x;
    player_y = other.player_y;
    cells = other.cells;
    enemy_positions = other.enemy_positions;
    player = other.player; 
}

void Game_field::move_from(Game_field&& other) noexcept{
    width = other.width;
    height = other.height;
    player_x = other.player_x;
    player_y = other.player_y;
    cells = std::move(other.cells);
    enemy_positions = std::move(other.enemy_positions);
    enemies = std::move(other.enemies);
    buildings = std::move(other.buildings);
    player = other.player;
    other.player = nullptr;
    other.width = 0;
    other.height = 0;
    other.player_x = -1;
    other.player_y = -1;
}

void Game_field::display_field() const{
    std::cout << "\n";
    for (int y = 0; y < height; ++y){
        for (int x = 0; x < width; ++x){
            const Cell& cell = cells[y][x];
            char symbol = '.';
            if (cell.get_has_player()) symbol = 'P';
            else if (cell.get_has_enemy()) symbol = 'E';
            else if (cell.get_has_ally()) symbol = 'A';
            else if (cell.get_has_building()){
                Building* building = get_building_at(x, y);
                if (building){
                    if (dynamic_cast<Enemy_tower*>(building)){
                        symbol = 'T';
                    } else{
                        symbol = 'B';
                    }
                } else {
                    symbol = 'B';
                }
            }
            else if (cell.get_type() == Cell_type::BLOCKED) symbol = '#';
            else if (cell.get_type() == Cell_type::SLOW) symbol = '~';
            else symbol = '.';
            std::cout << symbol << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}                                                                                               

void Game_field::clear_field(){
    enemies.clear();
    buildings.clear();
    enemy_positions.clear();
    player_x = -1;
    player_y = -1;
}


//заклинания

bool Game_field::is_enemy_or_building_at(int x, int y) const{
    if (!is_valid_position(x, y)){
        return false;
    }
    for (const auto& pos : enemy_positions){
        if (pos.first == x && pos.second == y){
            return true;
        }
    }
    for (const auto& building : buildings) {
        if (building->get_x() == x && building->get_y() == y && !building->is_destroyed()) {
            return true;
        }
    }
    return false;
}

void Game_field::perform_spell_attack(const std::string& attacker_name, int target_x, int target_y, int damage){    
    bool target_hit = false;
    for (size_t i = 0; i < enemies.size() && i < enemy_positions.size(); ++i){
        if (enemy_positions[i].first == target_x && enemy_positions[i].second == target_y){
            if (enemies[i] && enemies[i]->get_is_alive()){
                enemies[i]->take_damage(damage);
                std::cout << attacker_name << " наносит " << damage << " урона " << enemies[i]->get_name() << " заклинанием!\n";
                target_hit = true;
                if (!enemies[i]->get_is_alive() && player){
                    player->increase_score(SCORE_PER_KILL);
                    std::cout << "Враг побежден! +10 очков\n";
                }
            }
            break;
        }
    }
    if (!target_hit) {
        Building* building = get_building_at(target_x, target_y);
        if (building && !building->is_destroyed()){
            building->take_damage(damage);
            std::cout << attacker_name << " наносит " << damage << " урона " << building->get_name() << " заклинанием!\n";
            target_hit = true;    
            if (building->is_destroyed() && player){
                player->increase_score(SCORE_PER_DESTROY);
                std::cout << "Здание разрушено! +25 очков\n";
            }
        }
    }
    if (!target_hit) {
        std::cout << "Заклинание попадает в цель, но не находит подходящей цели!\n";
    }
}

void Game_field::perform_area_attack(const std::string& attacker_name, int center_x, int center_y, int damage, int area_size){    
    (void)attacker_name;
    int field_width = get_width();
    int field_height = get_height();
    std::cout << "Взрыв поражает область " << area_size << "x" << area_size << " вокруг (" << center_x << "," << center_y << ")!\n";
    int enemies_hit = 0;
    int buildings_hit = 0;

    int start_x = std::max(0, center_x - area_size / 2);
    int end_x = std::min(field_width - 1, center_x + area_size / 2);
    int start_y = std::max(0, center_y - area_size / 2);
    int end_y = std::min(field_height - 1, center_y + area_size / 2);
    
    for (size_t i = 0; i < enemies.size() && i < enemy_positions.size(); ++i){
        if (!enemies[i]) continue;
        int enemy_x = enemy_positions[i].first;
        int enemy_y = enemy_positions[i].second;

        if (enemy_x >= start_x && enemy_x <= end_x && enemy_y >= start_y && enemy_y <= end_y){
            if (enemies[i]->get_is_alive()){
                enemies[i]->take_damage(damage);
                std::cout << enemies[i]->get_name() << " получает " << damage << " урона!\n";
                enemies_hit++;
                if (!enemies[i]->get_is_alive() && player){
                    player->increase_score(SCORE_PER_KILL);
                    std::cout << "Враг побежден! +10 очков\n";
                }
            }
        }
    }
    for (size_t i = 0; i < buildings.size(); ++i){
        if (!buildings[i]) continue;
        int building_x = buildings[i]->get_x();
        int building_y = buildings[i]->get_y();

        if (building_x >= start_x && building_x <= end_x && 
            building_y >= start_y && building_y <= end_y){
            if (!buildings[i]->is_destroyed()){
                buildings[i]->take_damage(damage);
                std::cout << buildings[i]->get_name() << " получает " << damage << " урона!\n";
                buildings_hit++;
                
                if (buildings[i]->is_destroyed() && player){
                    player->increase_score(25);
                    std::cout << "Здание разрушено! +25 очков\n";
                }
            }
        }
    }
    std::cout << "Поражено врагов: " << enemies_hit << ", зданий: " << buildings_hit << "\n";
}


//здания

Building* Game_field::get_building_at(int x, int y) const{
    for (auto& building : buildings){
        if (building->get_x() == x && building->get_y() == y && !building->is_destroyed()){
            return building.get();
        }
    }
    return nullptr;
}

void Game_field::destroy_building(size_t index){
    if (index < buildings.size()){
        int x = buildings[index]->get_x();
        int y = buildings[index]->get_y();
        if (is_valid_position(x, y)){
            cells[y][x].set_type(Cell_type::EMPTY);
            cells[y][x].set_has_building(false);
        }
        buildings.erase(buildings.begin() + index);
    }
}
//вражеские

void Game_field::add_enemy_tower(const std::string& name, int x, int y){
    if (!is_valid_position(x, y) || !cells[y][x].is_empty()){
        std::cout << "Нельзя разместить башню в позиции (" << x << ", " << y << ")\n";
        return;
    }
    buildings.push_back(std::unique_ptr<Building>(new Enemy_tower(name, x, y, this)));
    cells[y][x].set_type(Cell_type::BUILDING);
    cells[y][x].set_has_building(true);
    std::cout << "Вражеская башня '" << name << "' построена в (" << x << ", " << y << ")\n";
}


//ловушки

void Game_field::damage_enemy_at_position(int x, int y, int damage){
    for (size_t i = 0; i < enemies.size(); ++i){
        if (enemy_positions[i].first == x && enemy_positions[i].second == y){
            if (enemies[i]->get_is_alive()){
                enemies[i]->take_damage(damage);
                std::cout << "Ловушка наносит " << damage << " урона " << enemies[i]->get_name() << "!\n";
                if (!enemies[i]->get_is_alive() && player){
                    player->increase_score(SCORE_PER_KILL);
                }
            }
            break;
        }
    }
}

void Game_field::place_trap(int x, int y, int damage){
    if (!is_valid_position(x, y) || !cells[y][x].can_move_to()){
        std::cout << "Нельзя разместить ловушку в позиции (" << x << ", " << y << ")\n";
        return;
    }
    traps.emplace_back(x, y, damage); // создает объект Trap сразу в векторе(не копирует)
}

void Game_field::update_traps(){
    for (size_t i = 0; i < traps.size(); ){
        traps[i].update(*this);
        if (!traps[i].is_active()){
            traps.erase(traps.begin() + i);
        } else{
            ++i;
        }
    } 
}

const std::vector<Trap>& Game_field::get_traps() const {
    return traps;
}

//союзники

std::pair<int, int> Game_field::get_ally_position(const Ally* ally) const{
    for (size_t i = 0; i < allies.size(); ++i){
        if (allies[i].get() == ally){
            return ally_positions[i];
        }
    }
    return {-1, -1};
}

void Game_field::add_ally(Ally* ally, int x, int y) {
    if (!is_valid_position(x, y) || !cells[y][x].can_move_to()) {
        std::cout << "Нельзя разместить союзника в позиции (" << x << ", " << y << ")\n";
        return;
    }
    allies.push_back(std::unique_ptr<Ally>(ally));
    ally_positions.push_back({x, y});
    cells[y][x].set_has_ally(true);
    ally->set_position(x, y);
    std::cout << "Союзник размещен в (" << x << ", " << y << ")\n";
}

void Game_field::update_allies(){
    for (size_t i = 0; i < allies.size(); ++i){
        if (allies[i] && allies[i]->is_alive()){
            allies[i]->update();
        }
    }
    for (size_t i = 0; i < allies.size(); ){
        if (!allies[i] || !allies[i]->is_alive()){
            int dead_x = ally_positions[i].first;
            int dead_y = ally_positions[i].second;
            cells[dead_y][dead_x].set_has_ally(false);
            allies.erase(allies.begin() + i);
            ally_positions.erase(ally_positions.begin() + i);
        } else{
            ++i;
        }
    }
}

bool Game_field::move_ally(Ally* ally, int new_x, int new_y){
    if (!is_valid_position(new_x, new_y)) return false;
    
    int ally_index = -1;
    for (size_t i = 0; i < allies.size(); ++i){
        if (allies[i].get() == ally) {
            ally_index = i;
            break;
        }
    }
    if (ally_index == -1) return false;
    if (!cells[new_y][new_x].can_move_to() || cells[new_y][new_x].get_has_enemy() || 
        cells[new_y][new_x].get_has_player() || cells[new_y][new_x].get_has_ally()){
        return false;
    }
    int old_x = ally_positions[ally_index].first;
    int old_y = ally_positions[ally_index].second;
    cells[old_y][old_x].set_has_ally(false);
    
    cells[new_y][new_x].set_has_ally(true);
    ally_positions[ally_index] = {new_x, new_y};
    ally->set_position(new_x, new_y);
    
    return true;
}

void Game_field::set_cell_type(int x, int y, Cell_type type) {
        if (is_valid_position(x, y)) {
            cells[y][x].set_type(type);
        }
    }