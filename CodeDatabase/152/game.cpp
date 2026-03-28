#include "game.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <cstdlib>
#include <ctime>
#include <cctype>
Game::Game()
    : field_(10, 10),
    player_(100, 10),
    hand_(3),
    game_over_(false),
    level_complete_(false)
{
    std::srand(static_cast<unsigned>(std::time(nullptr)));
}
bool Game::save(const std::string& filename) const noexcept {
    try {
        saveToFile(filename);
        std::cout << "Игра   успешно   сохранена   в  '" << filename << "'.\n";
        return true;
    }
    catch (const GameSaveException& e) {
        std::cerr << "Ошибка сохранения: " << e.what() << "\n";
        return false;
    }
}
bool Game::load(const std::string& filename) noexcept {
    try {
        loadFromFile(filename);
        std::cout << "Игра   успешно   загружена   из  '" << filename << "'.\n";
        return true;
    }
    catch (const GameLoadException& e) {
        std::cerr << " Ошибка   загрузки : " << e.what() << "\n";
        return false;
    }
}
void Game::run() {
    std::cout << "Добро пожаловать в игру!\n";
    std::cout << "Загрузить сохранённую игру? (y/n): ";
    char load_choice;
    std::cin >> load_choice;
    bool loaded = false;
    if (std::tolower(load_choice) == 'y') {
        std::cout << "Введите имя файла сохранения: ";
        std::string load_filename;
        std::cin >> load_filename;
        if (load(load_filename)) {
            loaded = true;
        }
    }
    if (!loaded) {
        start_level();
    }
}
void Game::start_level() {
    std::cout << "\n Запуск   уровня ...\n";
    enemies_.clear();
    player_.set_position(0, 0);
    field_.clear_cell(player_.row(), player_.col());
    field_.place_player(player_);
    for (int i = 0; i < 4; ++i) {
        int r = std::rand() % field_.rows();
        int c = std::rand() % field_.cols();
        if (r == player_.row() && c == player_.col()) { --i; continue; }
        enemies_.emplace_back(20, 10);
        enemies_.back().set_position(r, c);
        field_.update_cell(r, c, Cell::EntityType::kEnemy);
    }
}
void Game::execute(const PlayerAction& action) {
    bool player_acting = false;
    switch (action.type) {
    case ActionType::Move: {
        int nr = player_.row() + action.dr;
        int nc = player_.col() + action.dc;
        field_.move_player(nr, nc, player_);
        player_acting = true;
        break;
    }
    case ActionType::Attack: {
        Enemy* e = get_enemy_at(action.target_r, action.target_c);
        if (e) {
            e->take_damage(player_.damage());
            std::cout << " Вы   нанесли  " << player_.damage() << "  урона   врагу !\n";
            if (!e->is_alive()) field_.clear_cell(action.target_r, action.target_c);
            remove_dead_enemies();
        }
        else {
            std::cout << " В   этой   клетке   никого   нет .\n";
        }
        player_acting = true;
        break;
    }
    case ActionType::CastSpell: {
        if (action.spell_index >= hand_.size()) {
            std::cout << "Некорректный индекс заклинания.\n";
        }
        else {
            hand_.use_spell(action.spell_index, *this, field_, player_, action.target_r, action.target_c);
            player_acting = true;
        }
        break;
    }
    case ActionType::ShowSpells:
        hand_.show_spells();
        break;
    case ActionType::Save:
        if (!action.filename.empty()) save(action.filename);
        break;
    case ActionType::Load:
        if (!action.filename.empty()) load(action.filename);
        break;
    default:
        break;
    }
    if (player_acting) {
        enemies_turn();
    }
}
void Game::enemies_turn() {
    std::cout << "\n===  Ход   врагов  ===\n";
    for (auto& enemy : enemies_) {
        if (!enemy.is_alive()) continue;
        int er = enemy.row();
        int ec = enemy.col();
        int pr = player_.row();
        int pc = player_.col();
        int dr = (pr > er) ? 1 : (pr < er ? -1 : 0);
        int dc = (pc > ec) ? 1 : (pc < ec ? -1 : 0);
        int new_r = er + dr;
        int new_c = ec + dc;
        if (new_r == player_.row() && new_c == player_.col()) {
            player_.take_damage(enemy.damage());
            std::cout << "Враг в (" << er << "," << ec << ") атаковал игрока на "
                << enemy.damage() << "  урона ! "
                << " Здоровье   игрока : " << player_.health() << "\n";
            continue;
        }
        field_.move_enemy(new_r, new_c, enemy, player_);
    }
    remove_dead_enemies();
}
Enemy* Game::get_enemy_at(int row, int col) {
    for (auto& e : enemies_)
        if (e.row() == row && e.col() == col && e.is_alive()) return &e;
    return nullptr;
}
void Game::remove_dead_enemies() {
    enemies_.erase(std::remove_if(enemies_.begin(), enemies_.end(),
        [](const Enemy& e) { return !e.is_alive(); }), enemies_.end());
}
void Game::spawn_enemy(int health, int damage, int row, int col) {
    enemies_.emplace_back(health, damage);
    enemies_.back().set_position(row, col);
}
void Game::saveToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw GameSaveException("Не удалось открыть файл для сохранения: " + filename);
    }
    file << player_.health() << " " << player_.score() << " " << player_.row() << " " << player_.col() << "\n";
    file << enemies_.size() << "\n";
    for (const auto& enemy : enemies_) {
        file << enemy.health() << " " << enemy.damage() << " " << enemy.row() << " " << enemy.col() << "\n";
    }
    file << hand_.size() << "\n";
    for (size_t i = 0; i < hand_.size(); ++i) {
        file << hand_.spells_[i]->getType() << " " << hand_.spells_[i]->getDamage() << " " << hand_.spells_[i]->getRadius() << "\n";
    }
    file.close();
}
void Game::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw GameLoadException("Не удалось открыть файл для загрузки: " + filename);
    }
    int p_health, p_score, p_row, p_col;
    file >> p_health >> p_score >> p_row >> p_col;
    player_ = Player(p_health, player_.damage(), p_score); // Тут возникает ошибка из-за operator= = delete и const damage_
    player_.set_position(p_row, p_col);
    int num_enemies;
    file >> num_enemies;
    enemies_.clear();
    for (int i = 0; i < num_enemies; ++i) {
        int e_health, e_damage, e_row, e_col;
        file >> e_health >> e_damage >> e_row >> e_col;
        enemies_.emplace_back(e_health, e_damage);
        enemies_.back().set_position(e_row, e_col);
    }
    int num_spells;
    file >> num_spells;
    hand_.clear_spells();
    for (int i = 0; i < num_spells; ++i) {
        int type, damage, radius;
        file >> type >> damage >> radius;
        if (type == 0) {
            hand_.add_spell(std::make_unique<DirectDamageSpell>(damage, radius));
        }
        else if (type == 1) {
            hand_.add_spell(std::make_unique<AreaDamageSpell>(damage, radius));
        }
    }
    file.close();
}
bool Game::check_victory_condition() const {
    for (const auto& enemy : enemies_) {
        if (enemy.is_alive()) {
            return false;
        }
    }
    return true;
}
bool Game::check_defeat_condition() const {
    return !player_.is_alive();
}