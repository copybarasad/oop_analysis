#include "game_manager.hpp"
#include "game_controller.hpp"
#include "game_renderer.hpp"
#include "game_looper.hpp"
#include "console_input.hpp"
#include "save_and_load.hpp"
#include "event_logger.hpp"
#include <iostream>
#include <string>
#include <sstream>
#include <filesystem>
#include <vector>
#include <algorithm>
#include <random>
#include <cmath>

int Game_manager::get_input(const std::string& str, int min_v, int max_v){
    std::string input;
    bool flag = true;
    while(flag){
        std::cout << str;
        std::getline(std::cin, input);
        std::istringstream iss(input);
        int value;
        char trail;
        if(iss >> std::ws >> value){
            if(iss >> std::ws >> trail) std::cout << "Invalid input. Please enter a number between " << min_v << " and " << max_v << ".\n";
            else{
                if(value >= min_v && value <= max_v) return value;
                else std::cout << "Invalid input. Please enter a number between " << min_v << " and " << max_v << ".\n";
            }

        }
        else std::cout << "Invalid input. Please enter a number between " << min_v << " and " << max_v << ".\n";
    }
    return 1;
}

bool Game_manager::ask_yes_no(const std::string& str){
    bool flag = true;
    while(flag){
        std::cout << str;
        std::string line;
        std::getline(std::cin, line);
        std::istringstream iss(line);
        char choice;
        if(!(iss >> choice) || (iss >> std::ws).peek() != EOF){
            std::cout << "Invalid input. Please enter 'Y' or 'N'.\n";
            continue;
        }
        choice = std::tolower(static_cast<unsigned char>(choice));
        if(choice == 'y') return true;
        if(choice == 'n') return false;
        std::cout << "Invalid input. Please enter 'Y' or 'N'.\n";
    }
    return false;
}

void Game_manager::start(){
    std::cout << "Select logging method:\n";
    std::cout << "1) Log to console\n";
    std::cout << "2) Log to file 'logs.txt'\n";
    std::cout << "3) No logging\n";
    std::cout << "Enter your choice (1-3): ";
    int log_choice;
    bool flag = true;
    while(flag){
        std::string input;
        std::getline(std::cin, input);
        std::istringstream iss(input);
        if(iss >> log_choice && (iss >> std::ws).peek() == EOF){
            if(log_choice >= 1 && log_choice <= 3) break;
        }
        std::cout << "Invalid choice. Enter 1, 2 or 3: ";
    }
    Event_logger::Log_mode log_mode;
    switch(log_choice){
        case 1: log_mode = Event_logger::Log_mode::CONSOLE_ONLY; break;
        case 2: log_mode = Event_logger::Log_mode::FILE_ONLY; break;
        case 3: log_mode = Event_logger::Log_mode::NONE; break;
        default: log_mode = Event_logger::Log_mode::NONE;
    }
    while(flag){
        namespace fs = std::filesystem;
        std::vector<std::string> saves;
        if(fs::exists("saves") && fs::is_directory("saves")){
            for(auto& entry: fs::directory_iterator("saves")){
                if(entry.path().extension() == ".txt") saves.push_back(entry.path().filename().string());
            }
        }
        std::unique_ptr<Game_controller> game;
        if(!saves.empty()){
            std::cout << "Available saves:\n";
            for(size_t i = 0; i < saves.size(); i++){
                std::cout << (i + 1) << ") " << saves[i] << "\n";
            }
            std::cout << "For starting a new game, press any button except indexes.\n";
            std::cout << "Choose save (1-" << saves.size() << "): ";
            std::string line;
            std::getline(std::cin, line);
            std::istringstream iss(line);
            int choice = -1;
            if((iss >> choice) && (iss >> std::ws).peek() == EOF && choice >= 0 && choice <= static_cast<int>(saves.size())){
                if(choice > 0){
                    try{
                        Save_and_load loader;
                        game = loader.load_game(saves[choice - 1]);
                        std::cout << "Loaded: " << saves[choice - 1] << "\n";
                    } catch(const std::exception& e){
                        std::cerr << "Load failed: " << e.what() << "\n";
                    }
                }
            }
            else std::cout << "Starting a new game.\n";
        }
        if(!game){
            int height = get_input("Enter field height (10-25): ", 10, 25);
            int width = get_input("Enter field width (10-25): ", 10, 25);
            int enemy_count = get_input("Enter number of enemies (3-5): ", 3, 5);
            game = std::make_unique<Game_controller>(height, width, enemy_count);
            if(log_mode != Event_logger::Log_mode::NONE) {
                game->set_logging_enabled(log_mode, "logs.txt");
            }
        }
        else if(log_mode != Event_logger::Log_mode::NONE) game->set_logging_enabled(log_mode, "logs.txt");
        Game_renderer<Game_display> renderer(*game);
        Game_looper<Console_input> loop(*game, renderer.get_renderer());
        loop.run();
        if(!ask_yes_no("\nWould you like to play again? (Y/N): ")) break;
    }
}