#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include <iostream>
#include <string>
#include <thread>
#include <chrono>

template <typename TInputHandler, typename TVisualizer>
class GameManager {
private:
    TInputHandler input_handler_;
    TVisualizer visualizer_;
    Game& game_;

public:
    GameManager(Game& game) : game_(game) {
        input_handler_.load_config("controls.cfg");
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }

    void run() {
        bool app_running = true;
        while (app_running) {
            game_.reset_game();
            std::string help_str = input_handler_.get_controls_help_string();
            while (game_.is_running() && !game_.is_game_over()) {
                visualizer_.update(game_, help_str);
                auto command = input_handler_.handle_input();
                bool turn_spent = false;
                if (command) {
                    turn_spent = command->execute(game_);
                }
                if (turn_spent) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(250));
                }
                if (game_.get_field().are_all_enemies_dead()) {
                    visualizer_.update(game_, help_str);
                    std::this_thread::sleep_for(std::chrono::milliseconds(250));
                    game_.next_level(); 
                }
                if (turn_spent) {
                    game_.process_enemy_turns();
                    std::this_thread::sleep_for(std::chrono::milliseconds(250));
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
            }
            if (game_.is_game_over()) {
                visualizer_.update(game_, help_str);
                std::cout << "=== ИГРА ОКОНЧЕНА ===" << std::endl;
                std::cout << "Игрок погиб! Начать заново? (y/n): ";
                std::string line;
                std::getline(std::cin, line);
                if (line.empty() || std::tolower(line[0]) != 'y') {
                    app_running = false;
                }
            } else {
                app_running = false;
            }
        }
    }
};

#endif