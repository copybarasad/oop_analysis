#include "control_keys.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cctype>
#include <algorithm>
#include <filesystem>

Control_keys::Control_keys(){
    if(!std::filesystem::exists("control_keys.txt")){
        load_default();
        save_to_file("control_keys.txt");
        std::cout << "Default control keys file 'control_keys.txt' created.\n";
    }
    else{
        load_from_file("control_keys.txt");
        if(!is_valid){
            std::cout << "Invalid control keys file. Using defaults.\n";
            load_default();
        }
    }
}

void Control_keys::load_default(){
    command_to_key = {
        {Command_num::MOVE_UP, 'w'},
        {Command_num::MOVE_DOWN, 's'},
        {Command_num::MOVE_LEFT, 'a'},
        {Command_num::MOVE_RIGHT, 'd'},
        {Command_num::SWITCH_MELEE, 'c'},
        {Command_num::SWITCH_RANGED, 'r'},
        {Command_num::RANGED_ATTACK, 'f'},
        {Command_num::SHOW_HELP, 'h'},
        {Command_num::BUY_SPELL, 'b'},
        {Command_num::QUIT, 'q'},
        {Command_num::SAVE, 'u'},
        {Command_num::LOAD, 'l'},
        {Command_num::SPELL_CAST_1, '1'},
        {Command_num::SPELL_CAST_2, '2'},
        {Command_num::SPELL_CAST_3, '3'}
    };
    key_to_command.clear();
    for(auto& [cmd, key]: command_to_key){
        key_to_command[key] = cmd;
    }
    is_valid = true;
}

void Control_keys::save_to_file(const std::string& filename){
    std::ofstream file(filename);
    std::vector<std::pair<Command_num, std::string>> mapping ={
        {Command_num::MOVE_UP, "move_up"},
        {Command_num::MOVE_DOWN, "move_down"},
        {Command_num::MOVE_LEFT, "move_left"},
        {Command_num::MOVE_RIGHT, "move_right"},
        {Command_num::SWITCH_MELEE, "switch_melee"},
        {Command_num::SWITCH_RANGED, "switch_ranged"},
        {Command_num::RANGED_ATTACK, "ranged_attack"},
        {Command_num::SHOW_HELP, "show_help"},
        {Command_num::BUY_SPELL, "buy_spell"},
        {Command_num::QUIT, "quit"},
        {Command_num::SAVE, "save"},
        {Command_num::LOAD, "load"},
        {Command_num::SPELL_CAST_1, "spell_cast_1"},
        {Command_num::SPELL_CAST_2, "spell_cast_2"},
        {Command_num::SPELL_CAST_3, "spell_cast_3"}
    };
    for(auto& [cmd, name]: mapping){
        char key = get_key(cmd);
        file << name << "=" << key << "\n";
    }
}

void Control_keys::load_from_file(const std::string& filename){
    std::ifstream file(filename);
    if(!file.is_open()){
        is_valid = false;
        return;
    }
    command_to_key.clear();
    std::string line;
    while(std::getline(file, line)){
        if(line.empty()) continue;
        int eq = line.find('=');
        if(eq == std::string::npos || eq == 0 || eq == line.size() - 1) continue;
        std::string cmd_str = line.substr(0, eq);
        std::string key_str = line.substr(eq + 1);
        if(key_str.size() != 1) continue;
        char key = std::tolower(key_str[0]);

        Command_num cmd_num;
        if(cmd_str == "move_up") cmd_num = Command_num::MOVE_UP;
        else if(cmd_str == "move_down") cmd_num = Command_num::MOVE_DOWN;
        else if(cmd_str == "move_left") cmd_num = Command_num::MOVE_LEFT;
        else if(cmd_str == "move_right") cmd_num = Command_num::MOVE_RIGHT;
        else if(cmd_str == "switch_melee") cmd_num = Command_num::SWITCH_MELEE;
        else if(cmd_str == "switch_ranged") cmd_num = Command_num::SWITCH_RANGED;
        else if(cmd_str == "ranged_attack") cmd_num = Command_num::RANGED_ATTACK;
        else if(cmd_str == "show_help") cmd_num = Command_num::SHOW_HELP;
        else if(cmd_str == "buy_spell") cmd_num = Command_num::BUY_SPELL;
        else if(cmd_str == "quit") cmd_num = Command_num::QUIT;
        else if(cmd_str == "save") cmd_num = Command_num::SAVE;
        else if(cmd_str == "load") cmd_num = Command_num::LOAD;
        else if(cmd_str == "spell_cast_1") cmd_num = Command_num::SPELL_CAST_1;
        else if(cmd_str == "spell_cast_2") cmd_num = Command_num::SPELL_CAST_2;
        else if(cmd_str == "spell_cast_3") cmd_num = Command_num::SPELL_CAST_3;
        else continue;
        command_to_key[cmd_num] = key;
    }
    is_valid = validate();
}

bool Control_keys::validate(){
    key_to_command.clear();
    std::vector<bool> command_used(static_cast<int>(Command_num::COUNT), false);
    for(auto& [cmd, key]: command_to_key){
        int idx = static_cast<int>(cmd);
        if(idx < 0 || idx >= static_cast<int>(Command_num::COUNT)) return false;
        if(command_used[idx]) return false;
        command_used[idx] = true;
    }
    for(auto& [cmd, key]: command_to_key){
        if(key_to_command.count(key)) return false;
        key_to_command[key] = cmd;
    }
    for(int i = 0; i < static_cast<int>(Command_num::COUNT); i++){
        if(!command_used[i]) return false;
    }
    return true;
}

char Control_keys::get_key(Command_num cmd){
    auto i = command_to_key.find(cmd);
    return (i != command_to_key.end()) ? i->second : 0;
}

Control_keys::Command_num Control_keys::get_command(char key){
    key = std::tolower(key);
    auto pt = key_to_command.find(key);
    return (pt != key_to_command.end()) ? pt->second : Command_num::COUNT;
}
