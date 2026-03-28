#include "game.h"
#include "game_exceptions.h"
#include <iostream>
#include <thread>
#include <chrono>

Game::Game(int width, int height) 
    : field_(width, height), game_running_(true), current_level_(1) {}

void Game::reset_game() {
    int w = field_.get_width();
    int h = field_.get_height();
    field_ = Field(w, h);
    auto player = std::make_unique<Player>(100, 1, 1, 5, 50);
    field_.set_player(std::move(player));
    field_.add_enemy(std::make_unique<Enemy>(20, 5, 5, 5));
    field_.add_enemy(std::make_unique<Enemy>(20, 8, 8, 8));
    field_.add_enemy(std::make_unique<Enemy>(20, 7, 3, 7));
    field_.add_enemy_tower(std::make_unique<EnemyTower>(20, 12, 3, 4, 10));
    field_.add_enemy_tower(std::make_unique<EnemyTower>(20, 15, 7, 3, 8));
    current_level_ = 1;
    game_running_ = true;
}

bool Game::move_player(int dx, int dy) {
    return field_.move_player(dx, dy);
}

void Game::player_attack(int dx, int dy) {
    if (!field_.player_attack(dx, dy)) {
        std::cout << "Атака прошла мимо!" << std::endl;
    }
}

void Game::switch_weapon(bool to_melee) {
    Player* p = const_cast<Player*>(field_.get_player());
    if (p) {
        if (to_melee) p->switch_to_melee();
        else p->switch_to_ranged();
    }
}

void Game::save_game(const std::string& filename) {
    try {
        field_.save_to_file(filename);
        std::cout << "Игра сохранена." << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Ошибка сохранения: " << e.what() << std::endl;
    }
}

void Game::load_game(const std::string& filename) {
    try {
        field_.load_from_file(filename);
        std::cout << "Игра загружена." << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Ошибка загрузки: " << e.what() << std::endl;
    }
}

void Game::process_enemy_turns() {
    field_.move_enemies();
    field_.update_towers();
    Player* p = const_cast<Player*>(field_.get_player());
    if (p) p->restore_mana(5);
}

bool Game::is_game_over() const {
    return field_.is_game_over();
}

bool Game::handle_spell_cast_input() {
    Player* player = const_cast<Player*>(field_.get_player());
    if (!player || player->get_spell_hand().get_spell_count() == 0) {
        std::cout << "Нет заклинаний!" << std::endl;
        return false;
    }
    player->print_spells();
    std::cout << "Выберите заклинание (1-" << player->get_spell_hand().get_spell_count() << "): ";
    std::string line;
    std::getline(std::cin, line);
    try {
        int idx = std::stoi(line) - 1;
        if (idx < 0 || idx >= player->get_spell_hand().get_spell_count()) return false;
        std::cout << "Направление (w/a/s/d): ";
        std::getline(std::cin, line);
        if (line.empty()) return false;
        int dx = 0, dy = 0;
        char d = std::tolower(line[0]);
        if (d == 'w') dy = -1;
        else if (d == 's') dy = 1;
        else if (d == 'a') dx = -1;
        else if (d == 'd') dx = 1;
        else return false;
        Spell* s = player->get_spell_hand().get_spell(idx);
        int tx = player->get_x() + dx * s->get_range();
        int ty = player->get_y() + dy * s->get_range();
        return player->cast_spell(idx, field_, tx, ty);
    } catch (...) { return false; }
}

bool Game::buy_spell_interaction() {
    Player* p = const_cast<Player*>(field_.get_player());
    return p && p->buy_spell();
}

void Game::next_level() {
    std::cout << "\n========================================" << std::endl;
    std::cout << "       УРОВЕНЬ " << current_level_ << " ПРОЙДЕН! ПОБЕДА!       " << std::endl;
    std::cout << "========================================" << std::endl;
    current_level_++;
    std::unique_ptr<Player> survivor = field_.extract_player();
    if (!survivor) survivor = std::make_unique<Player>();
    level_up_menu(survivor.get());
    survivor->prepare_for_next_level();
    std::cout << "\nНажмите Enter, чтобы спуститься в подземелье..." << std::endl;
    std::string dummy;
    std::getline(std::cin, dummy);
    int new_w = field_.get_width() + 2; 
    int new_h = field_.get_height() + 1;
    if (new_w > 25) new_w = 25;
    if (new_h > 25) new_h = 25;
    field_ = Field(new_w, new_h);
    survivor->move(1 - survivor->get_x(), 1 - survivor->get_y());
    field_.set_player(std::move(survivor));
    int hp_bonus = current_level_ * 15;
    int dmg_bonus = current_level_ * 3;
    field_.add_enemy(std::make_unique<Enemy>(50 + hp_bonus, 5, 5, 5 + dmg_bonus));
    field_.add_enemy(std::make_unique<Enemy>(30 + hp_bonus, new_w-3, new_h-3, 8 + dmg_bonus));
    field_.add_enemy(std::make_unique<Enemy>(40 + hp_bonus, new_w/2, new_h/2, 7 + dmg_bonus));
    if (current_level_ >= 2) {
        field_.add_enemy_tower(std::make_unique<EnemyTower>(100 + hp_bonus, new_w-2, 3, 5, 10 + dmg_bonus));
    }
    if (current_level_ >= 3) {
         field_.add_enemy_tower(std::make_unique<EnemyTower>(100 + hp_bonus, 3, new_h-2, 5, 10 + dmg_bonus));
    }
    std::cout << "=== УРОВЕНЬ " << current_level_ << " НАЧИНАЕТСЯ ===" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
}

void Game::level_up_menu(Player* p) {
    if (!p) return;
    std::cout << "\nВыберите улучшение перед следующим уровнем:" << std::endl;
    std::cout << "1. Титан: +20 к Макс. Здоровью (и полное лечение)" << std::endl;
    std::cout << "2. Архимаг: +20 к Макс. Мане (и полное восстановление)" << std::endl;
    std::cout << "3. Жадность: +100 Очков" << std::endl;
    std::cout << "Ваш выбор (1-3): ";
    std::string choice;
    std::getline(std::cin, choice);
    char c = choice.empty() ? '1' : choice[0];
    if (c == '2') {
        p->upgrade_mana(20);
        std::cout << "Мана увеличена!" << std::endl;
    } else if (c == '3') {
        p->add_score(100);
        std::cout << "Очки получены!" << std::endl;
    } else {
        p->upgrade_health(20);
        std::cout << "Здоровье увеличено!" << std::endl;
    }
}