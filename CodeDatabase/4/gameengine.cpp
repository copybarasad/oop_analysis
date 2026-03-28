#include "gameengine.hpp"
#include "savemanager.hpp"
#include "exceptions.hpp"
#include <iostream>
#include <random>
#include <algorithm>

GameEngine::GameEngine() {}

void GameEngine::run() {
    show_main_menu();
}

void GameEngine::show_main_menu() {
    bool end = false;
    while (!end){
        std::cout << "=== Main Menu ===" << std::endl;
        std::cout << "1. New Game" << std::endl;
        std::cout << "2. Load Game" << std::endl;
        std::cout << "3. Exit" << std::endl;
        std::cout << "Choice: ";
        int choice;
        std::cin >> choice;
        switch (choice) {
            case 1:
                if (start_new_game()) {
                    run_level_loop();
                }
                break;
            case 2: {
                show_save_menu();
                std::string filename;
                std::cout << "Enter save filename: ";
                std::cin >> filename;
                try {
                    if (load_game(filename)) {
                        run_level_loop();
                    }
                } catch (const SaveLoadException& e) {
                    std::cout << "Load failed: " << e.what() << std::endl;
                }
                break;
            }
            case 3:
                std::cout << "Thanks for playing!" << std::endl;
                end = true;
            default:
                std::cout << "Invalid choice." << std::endl;
        }
    }
}

void GameEngine::show_save_menu() {
    std::cout << "=== Save Files ===" << std::endl;
    auto saves = SaveManager::list_save_files(SaveManager::SAVE_DIR);
    if (saves.empty()) {
        std::cout << "No saves found." << std::endl;
        return;
    }
    for (size_t i = 0; i < saves.size(); ++i) {
        std::cout << i << ": " << saves[i] << std::endl;
    }
}

bool GameEngine::start_new_game() {
    player = Hero();
    current_level_num = 1;
    int initial_enemy_count = 5;
    int initial_width = 15;
    int initial_height = 15;
    current_level_instance.emplace(current_level_num, initial_enemy_count, initial_width, initial_height);
    game_over = false;
    game_won = false;
    std::cout << "New game started!" << std::endl;
    return true;
}

bool GameEngine::load_game(const std::string& filename) {
    try {
        GameEngine loaded_state = SaveManager::load_from_file(filename);
        *this = std::move(loaded_state);
        std::cout << "Game loaded from " << filename << std::endl;
        return true;
    } catch (const SaveLoadException& e) {
        std::cout << "Load failed: " << e.what() << std::endl;
        return false;
    }
}

bool GameEngine::save_game(const std::string& filename) {
    try {
        SaveManager::save_to_file(*this, filename);
        std::cout << "Game saved to " << filename << std::endl;
        return true;
    } catch (const SaveLoadException& e) {
        std::cout << "Save failed: " << e.what() << std::endl;
        return false;
    }
}

void GameEngine::run_level_loop() {
    if (!current_level_instance) {
        std::cout << "Error: No active level." << std::endl;
        return;
    }

    Level& level = current_level_instance.value();
    GameController<ConsoleInput> controller;
    GameVisualizer<ConsoleRenderer> visualizer;

    controller.bind(player, level,*this);

    while (player.alive() && !level.is_complete()) {
        visualizer.update(level.field, player);
        if (!controller.handle_turn()) {
            std::cout << "Thanks for playing!" << std::endl;
            return;
        }
    }

    if (!player.alive()) {
        std::cout << "You died! Game Over." << std::endl;
        game_over = true;
        std::cout << "Restart? (y/n): ";
        char restart_choice;
        std::cin >> restart_choice;
        if (restart_choice == 'y' || restart_choice == 'Y') {
            start_new_game();
            run_level_loop();
        } else {
            game_over = true;
        }
    } else if (level.is_complete()) {
        std::cout << "Level " << current_level_num << " completed!" << std::endl;
        load_next_level();
        if (!game_won) {
            run_level_loop();
        }
    }
}

void GameEngine::load_next_level() {
    current_level_num++;
    player.health = player.max_health;
    size_t current_size = player.handle.size();
    size_t spells_to_remove = current_size / 2;
    if (spells_to_remove > 0) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::vector<size_t> indices(current_size);
        std::iota(indices.begin(), indices.end(), 0);
        std::shuffle(indices.begin(), indices.end(), gen);
        for (size_t i = 0; i < spells_to_remove; ++i) {
            player.handle.use_spell(indices[i] - i);
        }
        std::cout << "Half of your spells were lost!" << std::endl;
    }
    int enemy_count = 5 + (current_level_num - 1) * 2;
    int width = 10 + current_level_num * 2;
    int height = 10 + current_level_num * 2;
    current_level_instance.emplace(current_level_num, enemy_count, width, height);
    upgrade_player();
    if (current_level_num >= 5) {
        std::cout << "Congratulations! You have won the game!" << std::endl;
        game_won = true;
    }
}

void GameEngine::upgrade_player() {
    std::cout << "Choose an upgrade:" << std::endl;
    std::cout << "1. Increase Max Health (+25)" << std::endl;
    std::cout << "2. Increase Sword Damage (+20)" << std::endl;
    std::cout << "3. Increase Bow Damage (+10)" << std::endl;
    std::cout << "4. Increase Spell Hand Size (+1)" << std::endl;
    std::cout << "Choice: ";
    int choice;
    std::cin >> choice;
    switch (choice) {
        case 1:
            player.max_health += 25;
            player.health = player.max_health;
            std::cout << "Max Health increased!" << std::endl;
            break;
        case 2:
            player.sword_damage += 20;
            if (player.weapon == SWORD) {
                player.damage = player.sword_damage;
            }
            std::cout << "Sword damage increased!" << std::endl;
            break;
        case 3:
            player.bow_damage += 10;
            if (player.weapon == BOW) {
                player.damage = player.bow_damage;
            }
            std::cout << "Bow damage increased!" << std::endl;
            break;
        case 4:
            player.handle.increase_capacity(1);
            std::cout << "Spell Hand Size increased!" << std::endl;
            break;
        default:
            std::cout << "No upgrade selected." << std::endl;
    }
}