#include "console_input.hpp"
#include "commands.hpp"
#include <cctype>

bool Console_input::ask_position(int& x, int& y){
    std::string line;
    std::getline(std::cin, line);
    std::istringstream iss(line);
    return (iss >> x >> y) && (iss >> std::ws).peek() == EOF;
}

bool Console_input::ask_save_name(std::string& name){
    bool flag = true;
    while(flag){
        std::cout << "Enter save name (no spaces or dots): ";
        std::getline(std::cin, name);
        if(!name.empty() && name.find(' ') == std::string::npos && name.find('.') == std::string::npos) return true;
        std::cout << "Invalid name! Use any characters except spaces and dots.\n";
    }
    return false;
}

std::unique_ptr<Command> Console_input::get_next_command(bool& is_help, bool& is_quit, int hand_size){
    is_help = false;
    is_quit = false;
    std::cout << "\nEnter command: ";
    std::string input;
    std::getline(std::cin, input);
    if(input.empty()) return nullptr;
    char c = std::tolower(input[0]);
    Control_keys::Command_num cmd_num = control_keys.get_command(c);
    if(input.size() == 1){
        switch(cmd_num){
            case Control_keys::Command_num::MOVE_UP: return std::make_unique<Move_command>(Position(0, -1));
            case Control_keys::Command_num::MOVE_DOWN: return std::make_unique<Move_command>(Position(0, 1));
            case Control_keys::Command_num::MOVE_LEFT: return std::make_unique<Move_command>(Position(-1, 0));
            case Control_keys::Command_num::MOVE_RIGHT: return std::make_unique<Move_command>(Position(1, 0));
            case Control_keys::Command_num::SWITCH_MELEE: return std::make_unique<Switch_combat_style_command>(Combat_style::CLOSE_COMBAT);
            case Control_keys::Command_num::SWITCH_RANGED: return std::make_unique<Switch_combat_style_command>(Combat_style::RANGED_COMBAT);
            case Control_keys::Command_num::RANGED_ATTACK: return std::make_unique<Ranged_attack_command>();
            case Control_keys::Command_num::SHOW_HELP: {is_help = true; return nullptr;}
            case Control_keys::Command_num::BUY_SPELL: return std::make_unique<Buy_spell_command>();
            case Control_keys::Command_num::QUIT: {is_quit = true; return nullptr;}
            case Control_keys::Command_num::SAVE:{
                std::string name;
                if(ask_save_name(name)) return std::make_unique<Save_game_command>(name);
                return nullptr;
            }
            case Control_keys::Command_num::LOAD: return std::make_unique<Load_game_command>();
            case Control_keys::Command_num::SPELL_CAST_1:
            case Control_keys::Command_num::SPELL_CAST_2:
            case Control_keys::Command_num::SPELL_CAST_3:{
                int idx = static_cast<int>(cmd_num) - static_cast<int>(Control_keys::Command_num::SPELL_CAST_1);;
                if(idx >= hand_size){
                    std::cout << "No spell in that slot!\n";
                    return nullptr;
                }
                std::cout << "Enter target coordinates (x y): ";
                std::string line;
                std::getline(std::cin, line);
                std::istringstream iss(line);
                int x, y;
                if(!(iss >> x >> y) || (iss >> std::ws).peek() != EOF){
                    std::cout << "Invalid coordinates!\n";
                    return nullptr;
                }
                return std::make_unique<Spell_cast_command>(idx, Position(x, y));
            }
            default: return nullptr;
        }
    }
    return nullptr;
}