#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

#include "game.h"
#include "commands.h"
#include "console_input.h"

template<typename ConsoleInputType>

class Game_controller{
private:
    ConsoleInputType console_input;
public:
    Game_controller() : console_input(){}
    int level_command(Game& game){
        std::unique_ptr<Basic_command> command = console_input.create_level_command();
        if (command){
            return command->execute(game);
        }
        return 0;
    }

    void main_menu_command(Game& game){
        std::unique_ptr<Basic_command> command = console_input.create_main_menu_command();
        if (command){
            command->execute(game);
        }
    }

    void upgarde_command(Game& game){
        std::unique_ptr<Basic_command> command = console_input.create_upgrade_menu_command();
        if (command){
            command->execute(game);
        }
    }
};


#endif