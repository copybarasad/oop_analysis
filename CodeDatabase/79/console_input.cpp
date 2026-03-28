#include "console_input.h"

#include <string>
#include "game.h"

char Console_input::input_command(){
    std::string command;

    std::cout << "Введите команду: ";
    std::cin >> command;
    std::cout << std::endl;

    char com = command[0];
    return com;
}


std::unique_ptr<Basic_command> Console_input::create_level_command(){
    char com = input_command();
    switch(com){
        case 'w': return std::make_unique<Move_command>(Movement::Direction::Left);
        case 'a': return std::make_unique<Move_command>(Movement::Direction::Dawn);
        case 's': return std::make_unique<Move_command>(Movement::Direction::Right);
        case 'd': return std::make_unique<Move_command>(Movement::Direction::Up);
        case 'q': return std::make_unique<Attack_command>();
        case 'f': return std::make_unique<Use_spell_command>();
        case 'g': return std::make_unique<Buy_spell_command>();
        case 'c': return std::make_unique<Change_weapon_command>();
        case 'p': return std::make_unique<Save_game_command>();
        case 'm': return std::make_unique<Exit_game_command>();
        default:
            std::cout << "Vvedena nekorrektnaya comanda" << std::endl;
            return nullptr;
    }
}

std::unique_ptr<Basic_command> Console_input::create_main_menu_command(){
    char com = input_command();
    switch(com){
        case '1': return std::make_unique<Main_menu_commands>(com);
        case '2': return std::make_unique<Main_menu_commands>(com);
        case '3': return std::make_unique<Main_menu_commands>(com);
        default: return nullptr;
    }
}

std::unique_ptr<Basic_command> Console_input::create_upgrade_menu_command(){
    char com = input_command();
    switch(com){
        case '1': return std::make_unique<Upgrade_commands>(com);
        case '2': return std::make_unique<Upgrade_commands>(com);
        case '3': return std::make_unique<Upgrade_commands>(com);
        case '0': return std::make_unique<Upgrade_commands>(com);
        default: return nullptr;
    }
}