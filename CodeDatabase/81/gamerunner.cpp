#include "GameRunner.h"
#include <iostream>
#include <fstream>
#include <limits>

bool GameRunner::handle_startup_menu() {
    std::cout << "Начать новую игру (N) или загрузить (L)? (N/L): ";
    std::string command;
    if (!(std::cin >> command)) {
        return false;
    }
    
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (command == "N" || command == "n") {
        current_level = 1;
        is_loaded_game = false;
        std::ofstream ofs("buffer", std::ios::trunc);
        ofs.close();
        return true;
    } else if (command == "L" || command == "l") {
        is_loaded_game = true;
        return true; 
    } else {
        std::cout << "Неверная команда. Попробуйте снова.\n";
        return handle_startup_menu();
    }
}

void GameRunner::run_level(Game& game, ConsoleGameController& controller, ConsoleGameView& view) {
    bool level_passed = false;
    
    while (!level_passed) {
        view.update(game);
        
        int spells_needed = game.getPlayer().kills_to_spells();
        for (int i = 0; i < spells_needed; i++) {
            game.getHand().add_spell(0);
        }

        bool game_running = controller.run_turn(game);

        if (!game_running) {
            current_level = 0; 
            break; 
        }

        game.enemy_turn();
        game.tower_turn();

        if (game.getPlayer().getIs_dead()) {
            view.update(game);
            std::cout << "Игрок погиб. GAME OVER.\n";
            current_level = 0;
            break;
        }
        
        if (game.getCounterEnemies() == 0 && game.getCounterTowers() == 0) {
            view.update(game);
            std::cout << "Уровень " << current_level << " пройден!\n";
            
            std::ofstream ofs("buffer", std::ios::out | std::ios::trunc);
            if (ofs.is_open()) {
                game.getHand().save(ofs);
                ofs.close();
            } else {
                std::cerr << "ERROR: Не удалось сохранить руку в буфер.\n";
            }

            current_level++;
            level_passed = true;
        }
    }
}

int GameRunner::run() {
    if (!handle_startup_menu()) {
        return 0;
    }

    while (current_level != 0) {
        Game game(current_level);
        
        if (is_loaded_game) {
            std::cout << "Введите название файла c сохранением: ";
            std::string load_file;
            std::cin >> load_file;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            if (!game.load_game(load_file)) {
                std::cout << "Не удалось загрузить игру. Начинаем новую.\n";
                current_level = 1;
                game = Game(current_level);
                game.initialize(); 
            } else {
                current_level = game.getLevel(); 
            }
            is_loaded_game = false;
        } 
        else {
            game.initialize();
        }

        ConsoleGameController controller;
        ConsoleGameView view;

        run_level(game, controller, view);
    }
    
    return 0;
}