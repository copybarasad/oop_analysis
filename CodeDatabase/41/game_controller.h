#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

#include "game.h"
#include "input_handler.h"
#include <memory>
#include <iostream>

template<typename InputHandlerType>
class Game_controller {
private:
    Game* game;
    std::unique_ptr<InputHandlerType> input_handler;

public:
    Game_controller(Game* game_instance) 
        : game(game_instance), input_handler(std::make_unique<InputHandlerType>()) {}
    
    void process_input() {
        if (!game || !game->is_running()) return;
        
        auto command = input_handler->get_next_command();
        if (command) {
            std::string cmd_name = command->get_name();
            if (cmd_name.find("cast_spell") == 0) {
                std::cout << "Введите координаты цели (x y): ";
                int x, y;
                if (std::cin >> x >> y) {
                    size_t pos = cmd_name.find_last_of('_');
                    if (pos != std::string::npos) {
                        int spell_index = std::stoi(cmd_name.substr(pos + 1)) - 1;
                        command = std::make_unique<Cast_spell_command>(spell_index, x, y);
                    }
                } else {
                    std::cout << "Ошибка ввода координат!\n";
                    std::cin.clear();
                    std::cin.ignore(10000, '\n');
                    return;
                }
            }
            
            command->execute(*game);
            if (cmd_name.find("move") == 0 || 
                cmd_name == "attack" || 
                cmd_name.find("cast_spell") == 0) {
            }
        }
    }
    
    void set_game(Game* game_instance) {
        game = game_instance;
    }

    InputHandlerType& get_input_handler() {
        return *input_handler;
    }
};

#endif