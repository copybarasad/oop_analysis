#include "gamefield.h"
#include "game_exceptions.h"
#include <stdexcept>
#include <random>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <fstream>

Field::Field(int width, int height)
    : width_(width), height_(height) {
    if (width_ < 10) width_ = 10;
    if (height_ < 10) height_ = 10;
    if (width_ > 25) width_ = 25;
    if (height_ > 25) height_ = 25;
    
    initialize_grid();
    generate_walls();
}

void Field::initialize_grid() {
    grid_.resize(height_);
    for (int y = 0; y < height_; y++) {
        grid_[y].resize(width_);
        for (int x = 0; x < width_; x++) {
            grid_[y][x] = Cell(CellType::Empty);
        }
    }
}

void Field::generate_walls() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> wall_chance(0, 4);
    std::uniform_int_distribution<> wall_count((width_ * height_) / 10, (width_ * height_) / 5);

    int walls_to_place = wall_count(gen);
    int walls_placed = 0;

    while (walls_placed < walls_to_place) {
        for (int y = 1; y < height_ - 1 && walls_placed < walls_to_place; y++) {
            for (int x = 1; x < width_ - 1 && walls_placed < walls_to_place; x++) {
                if (x <= 2 && y <= 2) continue;

                if (wall_chance(gen) == 0 && grid_[y][x].get_type() == CellType::Empty) {
                    grid_[y][x] = Cell(CellType::Wall);
                    walls_placed++;
                }
            }
        }
    }
}

Field::Field(const Field& other) { 
    copy_from(other); 
}

Field::Field(Field&& other) noexcept = default;

Field& Field::operator=(const Field& other) { 
    if (this != &other) {
        copy_from(other); 
    }
    return *this; 
}

Field& Field::operator=(Field&& other) noexcept = default;

void Field::copy_from(const Field& other) {
    width_ = other.width_;
    height_ = other.height_;
    grid_ = other.grid_;

    if (other.player_) {
        player_ = std::make_unique<Player>(
            other.player_->get_health(), 
            other.player_->get_x(), 
            other.player_->get_y()
        );
        
        player_->set_health(other.player_->get_health());
        player_->add_score(other.player_->get_score() - 100);
        player_->add_experience(other.player_->get_experience());
        
        int mana_diff = other.player_->get_mana() - player_->get_mana();
        if(mana_diff > 0) player_->restore_mana(mana_diff);
        else player_->spend_mana(-mana_diff);

        const Weapon* other_weapon = other.player_->get_current_weapon();
        if (other_weapon) {
            if (other_weapon->is_ranged())
                player_->switch_to_ranged();
            else
                player_->switch_to_melee();
        }
        
    } else {
        player_.reset();
    }

    enemies_.clear();
    for (const auto& enemy : other.enemies_) {
        if (enemy) {
            auto new_enemy = std::make_unique<Enemy>(
                enemy->get_health(),
                enemy->get_x(),
                enemy->get_y(),
                enemy->get_damage()
            );
            new_enemy->set_health(enemy->get_health());
            enemies_.push_back(std::move(new_enemy));
        }
    }

    enemy_towers_.clear();
    for (const auto& tower : other.enemy_towers_) {
        if (tower) {
            auto new_tower = std::make_unique<EnemyTower>(
                tower->get_health(),
                tower->get_x(),
                tower->get_y(),
                tower->get_attack_range(),
                tower->get_damage()
            );
            new_tower->set_health(tower->get_health());
            new_tower->set_can_attack(tower->can_attack());
            enemy_towers_.push_back(std::move(new_tower));
        }
    }

    traps_ = other.traps_;
}

void Field::set_player(std::unique_ptr<Player> player) {
    player_ = std::move(player);
}

bool Field::move_player(int dx, int dy) {
    if (!player_) return false;

    int new_x = player_->get_x() + dx;
    int new_y = player_->get_y() + dy;

    if (!is_valid_position(new_x, new_y) || !can_move_to(new_x, new_y)) {
        return false;
    }

    for (const auto& enemy : enemies_) {
        if (enemy->is_alive() && enemy->is_at_position(new_x, new_y)) {
            std::cout << "Вы не можете войти в клетку с врагом!" << std::endl;
            return false;
        }
    }

    for (const auto& tower : enemy_towers_) {
        if (tower->is_alive() && tower->is_at_position(new_x, new_y)) {
            std::cout << "Вы не можете войти в клетку с вражеской башней!" << std::endl;
            return false;
        }
    }

    for (auto it = traps_.begin(); it != traps_.end();) {
        if (it->is_at_position(new_x, new_y)) {
            player_->take_damage(it->get_damage());
            std::cout << "Игрок наступил на ловушку и получил " << it->get_damage() << " урона!" << std::endl;
            it = traps_.erase(it);
            continue;
        }
        ++it;
    }

    player_->move(dx, dy);
    return true;
}

void Field::move_enemies() {
    if (!player_) return;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> move_chance(0, 3);

    for (auto& enemy : enemies_) {
        if (!enemy->is_alive()) continue;

        int player_x = player_->get_x();
        int player_y = player_->get_y();
        int enemy_x = enemy->get_x();
        int enemy_y = enemy->get_y();

        int dx = 0, dy = 0;

        if (move_chance(gen) > 0) {
            if (std::abs(player_x - enemy_x) >= std::abs(player_y - enemy_y)) {
                if (player_x > enemy_x) dx = 1;
                else if (player_x < enemy_x) dx = -1;
            } else {
                if (player_y > enemy_y) dy = 1;
                else if (player_y < enemy_y) dy = -1;
            }
        } else {
            switch(std::uniform_int_distribution<>(0, 3)(gen)) {
                case 0: dy = -1; break;
                case 1: dy = 1; break;
                case 2: dx = -1; break;
                case 3: dx = 1; break;
            }
        }

        if (dx != 0) dy = 0;
        if (dy != 0) dx = 0;

        int new_x = enemy_x + dx;
        int new_y = enemy_y + dy;

        for (auto it = traps_.begin(); it != traps_.end();) {
            if (it->is_at_position(new_x, new_y)) {
                enemy->take_damage(it->get_damage());
                std::cout << "Враг наступил на ловушку и получил " << it->get_damage() << " урона!" << std::endl;
                it = traps_.erase(it);
                continue;
            }
            ++it;
        }

        if ((dx != 0 && player_->is_at_position(new_x, enemy_y)) || 
            (dy != 0 && player_->is_at_position(enemy_x, new_y))) {
            player_->take_damage(enemy->get_damage());
            std::cout << "Враг атаковал игрока! Здоровье игрока: " << player_->get_health() << std::endl;
            continue;
        }

        if (is_valid_position(new_x, new_y) && can_move_to(new_x, new_y)) {
            bool enemy_collision = false;
            for (const auto& other_enemy : enemies_) {
                if (other_enemy.get() != enemy.get() && 
                    other_enemy->is_alive() && 
                    other_enemy->is_at_position(new_x, new_y)) {
                    enemy_collision = true;
                    break;
                }
            }
            
            if (!enemy_collision && !has_enemy_tower_at(new_x, new_y)) {
                enemy->move(dx, dy);
            }
        }
    }
}

void Field::update_towers() {
    for (auto& tower : enemy_towers_) {
        if (tower->is_alive()) {
            tower->update();
            tower->try_attack_player(*this, player_.get());
        }
    }
}

void Field::add_enemy(std::unique_ptr<Enemy> enemy) {
    enemies_.push_back(std::move(enemy));
}

void Field::add_enemy_tower(std::unique_ptr<EnemyTower> tower) {
    enemy_towers_.push_back(std::move(tower));
}

bool Field::player_attack(int dx, int dy) {
    if (!player_ || (dx == 0 && dy == 0)) return false;

    Player* p = const_cast<Player*>(player_.get());
    const Weapon* w = p->get_current_weapon();
    if (!w) return false;

    bool hit = false;
    
    for (int i = 1; i <= w->get_range(); i++) {
        int tx = p->get_x() + dx * i;
        int ty = p->get_y() + dy * i;

        if (!is_valid_position(tx, ty)) break;

        bool hit_here = false;

        for (auto& e : enemies_) {
            if (e->is_alive() && e->is_at_position(tx, ty)) {
                if (p->attack(e.get())) {
                    std::cout << "Враг атакован! HP: " << e->get_health() << std::endl;
                    if (!e->is_alive()) { 
                        std::cout << "Враг уничтожен!" << std::endl;
                        p->add_score(10); p->add_enemy_defeated(); 
                    }
                    hit = true;
                    hit_here = true;
                }
            }
        }

        for (auto& t : enemy_towers_) {
            if (t->is_alive() && t->is_at_position(tx, ty)) {
                if (p->attack(t.get())) {
                    std::cout << "Башня атакована! HP: " << t->get_health() << std::endl;
                    if (!t->is_alive()) {
                        std::cout << "Башня разрушена!" << std::endl;
                        p->add_score(20); 
                    }
                    hit = true;
                    hit_here = true;
                }
            }
        }

        if (hit_here) break;

        if (get_cell(tx, ty).get_type() == CellType::Wall) {
            break; 
        }
    }
    return hit;
}

bool Field::is_game_over() const {
    return !player_ || !player_->is_alive();
}

bool Field::is_valid_position(int x, int y) const {
    return x >= 0 && x < width_ && y >= 0 && y < height_;
}

bool Field::can_move_to(int x, int y) const {
    return is_valid_position(x, y) && grid_[y][x].is_passable();
}

bool Field::has_enemy_at(int x, int y) const {
    for (const auto& enemy : enemies_) {
        if (enemy->is_alive() && enemy->is_at_position(x, y)) return true;
    }
    return false;
}

bool Field::has_enemy_tower_at(int x, int y) const {
    for (const auto& tower : enemy_towers_) {
        if (tower->is_alive() && tower->is_at_position(x, y)) return true;
    }
    return false;
}

bool Field::has_trap_at(int x, int y) const {
    for (const auto& trap : traps_) {
        if (trap.is_at_position(x, y)) return true;
    }
    return false;
}

void Field::place_trap(int x, int y, int damage) {
    traps_.emplace_back(x, y, damage);
}

void Field::check_traps() {}

int Field::get_width() const { return width_; }
int Field::get_height() const { return height_; }

const Cell& Field::get_cell(int x, int y) const {
    if (!is_valid_position(x, y)) {
        static Cell default_cell(CellType::Wall);
        return default_cell;
    }
    return grid_[y][x];
}

const Player* Field::get_player() const { return player_.get(); }
const std::vector<std::unique_ptr<Enemy>>& Field::get_enemies() const { return enemies_; }
const std::vector<std::unique_ptr<EnemyTower>>& Field::get_enemy_towers() const { return enemy_towers_; }

void Field::print_enemies_info() const {
    std::cout << "=== ИНФОРМАЦИЯ О ВРАГАХ ===" << std::endl;
    int id = 1;
    for (const auto& e : enemies_) {
        if (e->is_alive()) {
            std::cout << "Враг " << id++ << " | HP: " << e->get_health()
                      << " | Урон: " << e->get_damage()
                      << " | Позиция: (" << e->get_x() << ", " << e->get_y() << ")\n";
        }
    }
    if (id == 1) std::cout << "Все враги повержены!\n";
}

void Field::print_towers_info() const {
    std::cout << "=== ИНФОРМАЦИЯ О БАШНЯХ ===" << std::endl;
    int id = 1;
    for (const auto& t : enemy_towers_) {
        if (t->is_alive()) {
            std::cout << "Башня  " << id++ << " | HP: " << t->get_health()
                      << " | Урон: " << t->get_damage()
                      << " | Дистанция: " << t->get_attack_range()
                      << " | Позиция: (" << t->get_x() << ", " << t->get_y() << ")\n";
        }
    }
    if (id == 1) std::cout << "Все башни разрушены!\n";
}

void Field::save_to_file(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw SaveLoadException("Не удалось открыть файл для записи: " + filename);
    }

    file << width_ << " " << height_ << "\n";
    for (int y = 0; y < height_; ++y) {
        for (int x = 0; x < width_; ++x) {
            file << (grid_[y][x].get_type() == CellType::Wall ? 1 : 0) << " ";
        }
        file << "\n";
    }
    file << "END_MAP\n";

    if (player_) {
        file << "1\n";
        player_->save(file);
    } else {
        file << "0\n";
    }
    file << " END_PLAYER\n";

    file << enemies_.size() << "\n";
    for (const auto& enemy : enemies_) {
        if (enemy->is_alive()) {
             file << "1 ";
             enemy->save(file);
             file << "\n";
        } else {
            file << "0 \n";
        }
    }
    file << "END_ENEMIES\n";

    file << enemy_towers_.size() << "\n";
    for (const auto& tower : enemy_towers_) {
        if (tower->is_alive()) {
            file << "1 ";
            tower->save(file);
            file << "\n";
        } else {
            file << "0 \n";
        }
    }
    file << "END_TOWERS\n";
    
    file << traps_.size() << "\n";
    for (const auto& trap : traps_) {
        file << trap.get_x() << " " << trap.get_y() << " " << trap.get_damage() << "\n";
    }
    file << "END_FILE\n";
}

void Field::load_from_file(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw SaveLoadException("Не удалось открыть файл: " + filename);
    }

    int temp_w, temp_h;
    if (!(file >> temp_w >> temp_h)) throw SaveLoadException("Ошибка чтения заголовка");

    if (temp_w < 10 || temp_w > 25 || temp_h < 10 || temp_h > 25) {
         throw SaveLoadException("Некорректные размеры поля");
    }

    std::vector<std::vector<Cell>> temp_grid(temp_h, std::vector<Cell>(temp_w));
    for (int y = 0; y < temp_h; ++y) {
        for (int x = 0; x < temp_w; ++x) {
            int type;
            if (!(file >> type)) throw SaveLoadException("Обрыв данных карты");
            temp_grid[y][x].set_type(type == 1 ? CellType::Wall : CellType::Empty);
        }
    }

    std::string marker;
    file >> marker;
    if (marker != "END_MAP") {
        throw SaveLoadException("Файл поврежден: ошибка в структуре карты (лишние или недостающие данные)");
    }

    std::unique_ptr<Player> temp_player;
    int has_player;
    if (!(file >> has_player)) throw SaveLoadException("Ошибка чтения флага игрока");
    
    if (has_player) {
        temp_player = std::make_unique<Player>();
        temp_player->load(file);
        if (file.fail()) throw SaveLoadException("Ошибка чтения параметров игрока");
    }

    file >> marker;
    if (marker != "END_PLAYER") {
        throw SaveLoadException("Файл поврежден: ошибка в данных игрока (лишние или недостающие параметры)");
    }

    std::vector<std::unique_ptr<Enemy>> temp_enemies;
    int enemies_count;
    if (!(file >> enemies_count)) throw SaveLoadException("Ошибка чтения кол-ва врагов");
    
    for(int i=0; i<enemies_count; ++i) {
        int is_alive;
        if (!(file >> is_alive)) throw SaveLoadException("Ошибка чтения статуса врага");
        if (is_alive) {
            auto enemy = std::make_unique<Enemy>(0, 0, 0);
            enemy->load(file);
            if (file.fail()) throw SaveLoadException("Ошибка параметров врага");
            temp_enemies.push_back(std::move(enemy));
        }
    }

    file >> marker;
    if (marker != "END_ENEMIES") {
        throw SaveLoadException("Файл поврежден: ошибка в списке врагов");
    }

    std::vector<std::unique_ptr<EnemyTower>> temp_towers;
    int towers_count;
    if (!(file >> towers_count)) throw SaveLoadException("Ошибка чтения кол-ва башен");
    
    for(int i=0; i<towers_count; ++i) {
        int is_alive;
        if (!(file >> is_alive)) throw SaveLoadException("Ошибка чтения статуса башни");
        if (is_alive) {
            auto tower = std::make_unique<EnemyTower>(0, 0, 0, 0, 0);
            tower->load(file);
            if (file.fail()) throw SaveLoadException("Ошибка параметров башни");
            temp_towers.push_back(std::move(tower));
        }
    }

    file >> marker;
    if (marker != "END_TOWERS") {
        throw SaveLoadException("Файл поврежден: ошибка в списке башен");
    }

    std::vector<Trap> temp_traps;
    int traps_count;
    if (!(file >> traps_count)) throw SaveLoadException("Ошибка чтения кол-ва ловушек");
    
    for(int i=0; i<traps_count; ++i) {
        int tx, ty, tdmg;
        if (!(file >> tx >> ty >> tdmg)) throw SaveLoadException("Ошибка параметров ловушки");
        temp_traps.emplace_back(tx, ty, tdmg);
    }

    file >> marker;
    if (marker != "END_FILE") {
        throw SaveLoadException("Файл поврежден: некорректное завершение файла");
    }

    width_ = temp_w;
    height_ = temp_h;
    grid_ = std::move(temp_grid);
    player_ = std::move(temp_player);
    enemies_ = std::move(temp_enemies);
    enemy_towers_ = std::move(temp_towers);
    traps_ = std::move(temp_traps);
}

bool Field::are_all_enemies_dead() const {
    for (const auto& enemy : enemies_) {
        if (enemy->is_alive()) return false;
    }
    for (const auto& tower : enemy_towers_) {
        if (tower->is_alive()) return false;
    }
    return true;
}

std::unique_ptr<Player> Field::extract_player() {
    return std::move(player_);
}