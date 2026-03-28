#include "field.h"
#include "logger.h"
#include "draw.h"
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iostream>

Field::Field(int field_width, int field_height)
    : width(field_width), height(field_height), player_x(-1), player_y(-1) {
    
    if (width < 10 || width > 25 || height < 10 || height > 25) {
        throw invalid_argument("Field dimensions must be between 10 and 25");
    }
    
    grid.resize(height, vector<CellType>(width, CellType::EMPTY));
}

Field::Field(const Field& other)
    : width(other.width), 
      height(other.height),
      grid(other.grid),
      player_x(other.player_x),
      player_y(other.player_y) {
    
    if (other.player) {
        player = make_unique<Player>(*other.player);
    }
    
    for (const auto& enemy : other.enemies) {
        enemies.push_back(make_unique<Enemy>(*enemy));
    }
    
    for (const auto& building : other.buildings) {
        buildings.push_back(make_unique<Building>(*building));
    }
}

Field::Field(Field&& other) noexcept
    : width(other.width),
      height(other.height),
      grid(move(other.grid)),
      player(move(other.player)),
      enemies(move(other.enemies)),
      buildings(move(other.buildings)),
      player_x(other.player_x),
      player_y(other.player_y) {
    
    other.player_x = -1;
    other.player_y = -1;
}

Field& Field::operator=(const Field& other) {
    if (this != &other) {
        width = other.width;
        height = other.height;
        grid = other.grid;
        player_x = other.player_x;
        player_y = other.player_y;
        
        if (other.player) {
            player = make_unique<Player>(*other.player);
        } else {
            player.reset();
        }
        
        enemies.clear();
        for (const auto& enemy : other.enemies) {
            enemies.push_back(make_unique<Enemy>(*enemy));
        }
        
        buildings.clear();
        for (const auto& building : other.buildings) {
            buildings.push_back(make_unique<Building>(*building));
        }
    }
    return *this;
}

Field& Field::operator=(Field&& other) noexcept {
    if (this != &other) {
        width = other.width;
        height = other.height;
        grid = move(other.grid);
        player = move(other.player);
        enemies = move(other.enemies);
        buildings = move(other.buildings);
        player_x = other.player_x;
        player_y = other.player_y;
        
        other.player_x = -1;
        other.player_y = -1;
    }
    return *this;
}

void Field::initialize_grid() {
    srand(static_cast<unsigned int>(time(nullptr)));
    walls();
    slowdown_cells();
}

void Field::walls() {
    int wall_count = (width * height) / 10;
    
    for (int i = 0; i < wall_count; ++i) {
        int x = rand() % width;
        int y = rand() % height;
        
        if (grid[y][x] == CellType::EMPTY) {
            grid[y][x] = CellType::WALL;
        }
    }
}

void Field::slowdown_cells() {
    int slowdown_count = (width * height) / 20;
    
    for (int i = 0; i < slowdown_count; ++i) {
        int x = rand() % width;
        int y = rand() % height;
        
        if (grid[y][x] == CellType::EMPTY) {
            grid[y][x] = CellType::SLOWDOWN;
        }
    }
}

bool Field::is_valid_position(int x, int y) const {
    return x >= 0 && x < width && y >= 0 && y < height;
}

bool Field::is_cell_occupied(int x, int y) const {
    if (!is_valid_position(x, y)) {
        return true;
    }

    if (grid[y][x] == CellType::WALL) {
        return true;
    }

    if (x == player_x && y == player_y && player && player->is_alive()) {
        return true;
    }

    for (const auto& enemy : enemies) {
        if (enemy->get_x() == x && enemy->get_y() == y && enemy->is_alive()) {
            return true;
        }
    }

    for (const auto& ally : allies) {
        if (ally->get_x() == x && ally->get_y() == y && ally->is_alive()) {
            return true;
        }
    }

    for (const auto& tower : enemy_towers) {
        if (tower->get_x() == x && tower->get_y() == y) {
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

Enemy* Field::get_enemy_at(int x, int y) {
    for (auto& enemy : enemies) {
        if (enemy->get_x() == x && enemy->get_y() == y && enemy->is_alive()) {
            return enemy.get();
        }
    }
    return nullptr;
}

void Field::create_player(int x, int y, int health, int close_dmg, int far_dmg) {
    if (!is_valid_position(x, y) || is_cell_occupied(x, y)) {
        throw invalid_argument("Invalid player position");
    }
    if (health <= 0 || close_dmg < 0 || far_dmg < 0) {
        throw invalid_argument("Invalid player stats: health must be > 0, damages >= 0");
    }
    player = make_unique<Player>(health, close_dmg, far_dmg);
    player_x = x;
    player_y = y;
}

void Field::create_enemy(int x, int y, int health, int damage) {
    if (!is_valid_position(x, y) || is_cell_occupied(x, y)) {
        return;
    }

    if (health <= 0 || damage < 0) {
        throw invalid_argument("Invalid enemy stats: health > 0, damage >= 0");
    }
    
    if (x == player_x && y == player_y) {
        return;
    }

    enemies.push_back(make_unique<Enemy>(health, damage, x, y));
}

void Field::create_building(int x, int y, int spawn_interval, int health) {
    if (is_cell_occupied(x, y)) {
        throw invalid_argument("Invalid building position");
    }

    if (x == player_x && y == player_y) {
        throw invalid_argument("Can`t place a building on  a player position");
    }
    
    grid[y][x] = CellType::BUILD;
    buildings.push_back(make_unique<Building>(x, y, spawn_interval, health));
}

bool Field::move_player(int dx, int dy) {
    if (!player || !player->is_alive() || player->is_currently_slowed()) {
        return false;
    }

    int new_x = player_x + dx;
    int new_y = player_y + dy;
    
    if (!is_valid_position(new_x, new_y) || is_cell_occupied(new_x, new_y)) {
        return false;
    }

    Enemy* enemy = get_enemy_at(new_x, new_y);
    if (enemy) {
        return false;
    }

    int old_x = player_x, old_y = player_y;
    player_x = new_x;
    player_y = new_y;

    if (auto lg = global_logger()) {
        lg->log("Player moved from (" + std::to_string(old_x) + "," +
                std::to_string(old_y) + ") to (" +
                std::to_string(new_x) + "," + std::to_string(new_y) + ")");
    }
    
    if (grid[new_y][new_x] == CellType::SLOWDOWN) {
        player->apply_slowdown();
    }
    
    return true;
}

bool Field::is_player_slowed() const {
    if (player) {
        return player->is_currently_slowed();
    }
    return false;
}

void Field::move_enemies() {
    if (!player || !player->is_alive()) {
        throw invalid_argument("Can`t move an enemy there");
    }
    
    for (auto& enemy : enemies) {
        if (!enemy->is_alive()) {
            continue;
        }
        
        int enemy_x = enemy->get_x();
        int enemy_y = enemy->get_y();
        
        int dx = 0, dy = 0;
        
        if (player_x > enemy_x) dx = 1;
        else if (player_x < enemy_x) dx = -1;
        
        if (player_y > enemy_y) dy = 1;
        else if (player_y < enemy_y) dy = -1;
        
        int new_x = enemy_x + dx;
        int new_y = enemy_y + dy;
        
        if (new_x == player_x && new_y == player_y) {
            int dmg = enemy->get_damage();
            player->take_damage(dmg);
            if (auto lg = global_logger()) {
                lg->log("Enemy at (" + std::to_string(enemy_x) + "," +
                        std::to_string(enemy_y) + ") hit player for " +
                        std::to_string(dmg));
            }
            continue;
        }
        
        if (!is_cell_occupied(new_x, new_y) && !get_enemy_at(new_x, new_y)) {
            enemy->set_position(new_x, new_y);
            check_traps(new_x, new_y);
        }
    }
}

void Field::create_buildings() {
    for (auto& building : buildings) {
        building->did_step();
        
        if (building->do_create_enemy()) {
            int dx = building->get_x();
            int dy = building->get_y();
            
            int directions[][2] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
            
            for (auto& dir : directions) {
                int spawn_x = dx + dir[0];
                int spawn_y = dy + dir[1];
                
                if (is_valid_position(spawn_x, spawn_y) && 
                    !is_cell_occupied(spawn_x, spawn_y) &&
                    !get_enemy_at(spawn_x, spawn_y) &&
                    !(spawn_x == player_x && spawn_y == player_y)) {
                    
                    create_enemy(spawn_x, spawn_y, 30, 5);
                    building->reset_timer();
                    break;
                }
            }
        }
    }
}

void Field::player_attack(int target_x, int target_y) {
    if (!player || !player->is_alive()) {
        throw invalid_argument("Can`t attack an enemy");
    }
    
    Enemy* enemy = get_enemy_at(target_x, target_y);
    if (enemy) {
        int distance = abs(target_x - player_x) + abs(target_y - player_y);
        
        if (player->get_mode() == Mode::CLOSE && distance == 1) {
            int dmg = player->get_current_damage();
            enemy->take_damage(dmg);
            if (auto lg = global_logger()) {
                lg->log("Player dealt " + std::to_string(dmg) +
                        " damage to enemy at (" +
                        std::to_string(target_x) + "," +
                        std::to_string(target_y) + ")");
            }
        } else if (player->get_mode() == Mode::FAR && distance <= 3) {
            enemy->take_damage(player->get_current_damage());
        }
    }
}

void Field::finish_turn() {
    if (player) {
        player->slowdown_turn();
        player->clear_mode_cooldown();
    }
    update_towers();
    update_allies();

    int killed_count = 0;
    for (const auto& enemy : enemies) {
        if (!enemy->is_alive()) {
            killed_count++;
        }
    }

    if (player && killed_count > 0) {
        player->add_score(killed_count * 100);
        for (int i = 0; i < killed_count; ++i) {
            player->increment_killed();
        }
    }

    enemies.erase(
        remove_if(enemies.begin(), enemies.end(),
            [](const auto& e) { return !e->is_alive(); }),
        enemies.end()
    );

   auto build_it = remove_if(buildings.begin(), buildings.end(),
        [](const auto& b) { return b->is_destroyed(); });
    int destroyed_buildings = distance(build_it, buildings.end());
    if (player && destroyed_buildings > 0) {
        player->add_score(destroyed_buildings * 500);
    }

    for (auto it = build_it; it != buildings.end(); ++it) {
        int bx = (*it)->get_x();
        int by = (*it)->get_y();
        grid[by][bx] = CellType::EMPTY;
    }
    buildings.erase(build_it, buildings.end());
}

void Field::display() const {
    Renderer r;
    r.render_field(*this);
}

bool Field::is_game_over() const {
    return !player || !player->is_alive();
}

int Field::get_width() const {
    return width;
}

int Field::get_height() const {
    return height;
}

CellType Field::get_cell_type(int x, int y) const {
    if (!is_valid_position(x, y)) {
        return CellType::WALL;
    }
    return grid[y][x];
}

void Field::switch_player_mode() {
    if (player) {
        player->switch_mode();
    }
}

int Field::get_player_score() const {
    if (player) {
        return player->get_score();
    }
    return 0;
}

void Field::player_use_spell(size_t spell_idx, int target_x, int target_y) {
    if (!player) return;
    player->get_hand().use_spell(spell_idx, *this, player_x, player_y, target_x, target_y);
}

const vector<unique_ptr<Building>>& Field::get_buildings() const {
    return buildings;
}

void Field::place_trap(int x, int y, int damage) {
    traps.push_back(Trap(x, y, damage));
    grid[y][x] = CellType::TRAP;
}

void Field::check_traps(int enemy_x, int enemy_y) {
    for (auto it = traps.begin(); it != traps.end(); ) {
        if (it->x == enemy_x && it->y == enemy_y) {
            Enemy* enemy = get_enemy_at(enemy_x, enemy_y);
            if (enemy && enemy->is_alive()) {
                enemy->take_damage(it->damage);
                std::cout << "Enemy stepped on trap! -" << it->damage << " HP\n";
            }
            grid[enemy_y][enemy_x] = CellType::EMPTY;
            it = traps.erase(it);
        } else {
            ++it;
        }
    }
}

void Field::create_tower(int x, int y, int dmg, int rad, int cd) {
    if (!is_valid_position(x, y) || is_cell_occupied(x, y)) {
        throw invalid_argument("Invalid tower position");
    }
    enemy_towers.push_back(make_unique<Tower>(x, y, dmg, rad, cd));
}

void Field::update_towers() {
    for (auto& tower : enemy_towers) {
        tower->update();
        tower->try_attack(*this);
    }
}

Player* Field::get_player() {
    return player.get();
}

const Player* Field::get_player() const {
    return player.get();
}

int Field::get_player_x() const {
    return player_x;
}

int Field::get_player_y() const {
    return player_y;
}

void Field::create_ally(int x, int y, int hp, int dmg) {
    allies.push_back(make_unique<Ally>(x, y, hp, dmg));
}

void Field::update_allies() {
    for (auto& ally : allies) {
        ally->update(*this);
    }

    allies.erase(
        std::remove_if(allies.begin(), allies.end(),
            [](const auto& a){ return !a->is_alive(); }),
        allies.end()
    );
}

const vector<unique_ptr<Enemy>>& Field::get_enemies() const {
    return enemies;
}


void Field::set_player_position(int x, int y) {
    player_x = x;
    player_y = y;
}

const vector<unique_ptr<Tower>>& Field::get_towers() const {
    return enemy_towers;
}

char Field::get_tile_symbol(int x, int y) const {
    // игрок
    if (player && player_x == x && player_y == y) {
        return 'P';
    }

    // враги
    for (const auto& enemy : enemies) {
        if (enemy->get_x() == x && enemy->get_y() == y && enemy->is_alive()) {
            return 'E';
        }
    }

    // союзники
    for (const auto& ally : allies) {
        if (ally->get_x() == x && ally->get_y() == y && ally->is_alive()) {
            return 'A';
        }
    }

    // ловушки
    for (const auto& trap : traps) {
        if (trap.x == x && trap.y == y) {
            return 'T';
        }
    }

    // здания / башни
    if (grid[y][x] == CellType::BUILD) {
        bool is_tower = false;
        for (const auto& t : enemy_towers) {
            if (t->get_x() == x && t->get_y() == y) {
                is_tower = true;
                break;
            }
        }
        return is_tower ? 'X' : 'B';
    }

    switch (grid[y][x]) {
        case CellType::EMPTY:     return '.';
        case CellType::WALL:      return '#';
        case CellType::SLOWDOWN:  return '~';
        case CellType::TRAP:      return 'T';
        default:                  return '.';
    }
}

void Field::set_tile_from_symbol(int x, int y, char c) {
    switch (c) {
        case '.':
            grid[y][x] = CellType::EMPTY;
            break;

        case '#':
            grid[y][x] = CellType::WALL;
            break;

        case '~':
            grid[y][x] = CellType::SLOWDOWN;
            break;

        case 'T':
            grid[y][x] = CellType::TRAP;
            break;
        default:
            grid[y][x] = CellType::EMPTY;
            break;
    }
}

int Field::get_level() const {
    return game_level;
}

void Field::set_level(int lvl) {
    game_level = lvl;
}

void Field::advance_level() {
    ++game_level;
}

bool Field::is_level_completed() const {
    return player && player->get_score() >= 1000 * game_level;
}