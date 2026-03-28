#include "GameManager.h"
#include <iostream>
#include <fstream>
#include <random>
#include <set>
#include <windows.h>
#include "GameController.h"
#include "GameVisualizer.h"
#include "ConsoleInputHandler.h"
#include "ConsoleRenderer.h"

// Параметры
#define BASE_SIZE_X 16
#define BASE_SIZE_Y 10
#define WALLS_COUNT 16
#define PLAYER_DAMAGE 50
#define PLAYER_START_X 0
#define PLAYER_START_Y 0
#define PLAYER_HEALTH 350
#define PLAYER_FAR_ATTACK_REACH 3
#define START_SPELL_COUNT 3
#define SHOP_SPELL_COUNT 5
#define SPELL_DAMAGE 50, 100
#define SPELL_RANGE 2, 5
#define SPELL_PRICE 100, 250
#define ENEMY_COUNT 3
#define ENEMY_DAMAGE 25
#define ENEMY_HEALTH 150
#define TOWER_COUNT 1
#define TOWER_DAMAGE 50
#define TOWER_HEALTH 300
#define TOWER_DELAY 3
#define TOWER_RANGE 4
#define UPGRADE_HP_BONUS 50
#define UPGRADE_DMG_BONUS 15

void GameManager::clean_up() {
    delete game; game = nullptr;
    delete field; field = nullptr;
    delete player; player = nullptr;
    delete playerHand; playerHand = nullptr;
    enemies.clear();
    towers.clear();
}

void GameManager::run_menu() {
    SetConsoleOutputCP(CP_UTF8);
    bool exit = false;
    while(!exit) {
        std::cout << "\n=== Меню ===\n1. Новая игра\n2. Загрузить игру\n3. Выйти\n> ";
        std::string input;
        std::getline(std::cin, input);
        if (input == "1") {
            start_new_game();
            game_loop();
        } else if (input == "2") {
            CreateDirectoryA("saves", NULL);
            WIN32_FIND_DATAA fd;
            HANDLE h = FindFirstFileA("saves/*.json", &fd);
            std::cout << "\nSaves:\n";
            if (h != INVALID_HANDLE_VALUE) {
                do {
                    std::string name = fd.cFileName;
                    name = name.substr(0, name.size() - 5);
                    std::cout << " - " << name << "\n";
                } while (FindNextFileA(h, &fd));
                FindClose(h);
            }
            std::cout << "\nИмя файла: ";
            std::string fn;
            std::getline(std::cin, fn);
            if (load_game(fn)) game_loop();
        } else if (input == "3") {
            exit = true;
        }
    }
}

void GameManager::start_new_game() {
    clean_up();
    current_level = 1;
    setup_level(current_level);
}

void GameManager::game_loop() {
    if (!game) return;

    GameVisualizer<ConsoleRenderer> visualizer;
    visualizer.set_game(game);
    game->set_observer(&visualizer);

    GameController<ConsoleInputHandler> controller(*game);

    controller.run();

    if (game->is_victory_state()) {
        clear_level_resources();
        level_up_menu();
        game_loop(); 
    } else {
        std::cout << "Игра окончена, возвращение в меню.\n";
    }
}

bool GameManager::save_game(std::string filename) {
    CreateDirectoryA("saves", NULL);
    if (filename.ends_with(".json")) filename = filename.substr(0, filename.size() - 5);
    std::string fullpath = "saves/" + filename + ".json";
    
    std::ofstream out(fullpath);
    if (!out.is_open()) return false;
    json j;
    j["field"] = field->to_json();
    j["player"] = player->to_json();
    j["enemies"] = json::array();
    for(auto& e : enemies) j["enemies"].push_back(e.to_json());
    j["towers"] = json::array();
    for(auto& t : towers) j["towers"].push_back(t.to_json());
    j["game"] = game->to_json();
    out << j.dump(4);
    out.close();
    if(game) game->log("Game saved.");
    return true;
}

bool GameManager::load_game(std::string filename) {
    std::string fullpath = "saves/" + filename;
    if (!fullpath.ends_with(".json")) fullpath += ".json";
    std::ifstream in(fullpath);
    if (!in.is_open()) { std::cout << "Сохранение отсутствует.\n"; return false; }

    try {
        json j; in >> j;
        if (!validate_save_json(j)) return false;
        clean_up();

        playerHand = new PlayerHand();
        player = new Player({0,0}, 1, 1, 1, *playerHand);
        field = new Field();

        if (!field->from_json(j.at("field"))) throw std::exception();
        if (!player->from_json(j.at("player"))) throw std::exception();
        for (auto& e_json : j.at("enemies")) {
            Enemy e; e.from_json(e_json); enemies.push_back(e);
        }
        for (auto& t_json : j.at("towers")) {
            EnemyTower t; t.from_json(t_json); towers.push_back(t);
        }
        game = new Game(*player, enemies, *field, towers, 1, this);
        if (!game->from_json(j.at("game"))) throw std::exception();
        this->current_level = game->get_level();
        return true;
    } catch (...) {
        clean_up();
        return false;
    }
}

void GameManager::clear_level_resources() {
    delete game; game = nullptr;
    delete field; field = nullptr;
    enemies.clear();
    towers.clear();
}

void GameManager::setup_level(int level) {
    int size_x = std::min(30, BASE_SIZE_X + (level - 1) * 3);
    int size_y = std::min(30, BASE_SIZE_Y + (level - 1) * 3);
    int enemy_hp = ENEMY_HEALTH + (level - 1) * 30;
    int enemy_dmg = ENEMY_DAMAGE + (level - 1) * 5;
    int enemy_count = ENEMY_COUNT + (level / 2);

    if (!player) {
        playerHand = new PlayerHand(START_SPELL_COUNT, SHOP_SPELL_COUNT, {SPELL_DAMAGE}, {SPELL_RANGE}, {SPELL_PRICE});
        player = new Player({PLAYER_START_X, PLAYER_START_Y}, PLAYER_HEALTH, PLAYER_DAMAGE, PLAYER_FAR_ATTACK_REACH, *playerHand);
    } else {
        player->setPosition({PLAYER_START_X, PLAYER_START_Y});
    }

    std::random_device rd; std::mt19937 gen(rd());
    std::uniform_int_distribution<> x_dist(0, size_x - 1);
    std::uniform_int_distribution<> y_dist(0, size_y - 1);
    std::set<std::pair<int, int>> occupied;
    occupied.insert(player->getPosition());

    for (int i = 0; i < enemy_count; i++) {
        std::pair<int, int> pos;
        do { pos.first = x_dist(gen); pos.second = y_dist(gen); } while (occupied.find(pos) != occupied.end());
        enemies.emplace_back(pos, enemy_hp, enemy_dmg);
        occupied.insert(pos);
    }
    for (int i = 0; i < TOWER_COUNT; i++) {
        std::pair<int, int> pos;
        do { pos.first = x_dist(gen); pos.second = y_dist(gen); } while (occupied.find(pos) != occupied.end());
        towers.emplace_back(pos, TOWER_HEALTH + (level - 1) * 20, TOWER_HEALTH + (level - 1) * 20, TOWER_DAMAGE + (level - 1) * 5, TOWER_RANGE, TOWER_DELAY);
        occupied.insert(pos);
    }

    field = new Field(size_x, size_y, enemies, towers, *player, WALLS_COUNT + (level - 1) * 3);
    game = new Game(*player, enemies, *field, towers, level, this);
}

void GameManager::level_up_menu() {
    std::cout << "\n=== Новый уровень: " << (current_level + 1) << " ===\n";
    player->full_heal();
    playerHand->remove_random_half();
    std::cout << "Ваши силы восстановлены. Часть заклинаний потеряна.\n";

    std::cout << "Улучшения:\n1. HP +" << UPGRADE_HP_BONUS << "\n2. Урон +" << UPGRADE_DMG_BONUS << "\n> ";
    int choice = 0;
    std::string input;
    while (true) {
        std::getline(std::cin, input);
        try {
            choice = std::stoi(input);
            if (choice == 1) { player->increase_max_health(UPGRADE_HP_BONUS); break; }
            else if (choice == 2) { player->increase_damage(UPGRADE_DMG_BONUS); break; }
        } catch(...) {}
    }
    current_level++;
    setup_level(current_level);
}

bool GameManager::validate_save_json(json& j) {
   if (!j.is_object()) return false;
   if (!j.contains("field") || !j.contains("player") || !j.contains("enemies") || !j.contains("towers") || !j.contains("game")) return false;
   return true;
}
