#include "input_handler.h"
#include "command.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

Input_handler::Input_handler() {
    setup_default_bindings();
}

void Input_handler::setup_default_bindings() {
    key_bindings['w'] = std::make_unique<Move_command>(0, -1);
    key_bindings['s'] = std::make_unique<Move_command>(0, 1);
    key_bindings['a'] = std::make_unique<Move_command>(-1, 0);
    key_bindings['d'] = std::make_unique<Move_command>(1, 0);
    key_bindings['e'] = std::make_unique<Attack_command>();
    key_bindings['q'] = std::make_unique<Switch_mode_command>();
    key_bindings['1'] = std::make_unique<Cast_spell_command>(0);
    key_bindings['2'] = std::make_unique<Cast_spell_command>(1);
    key_bindings['3'] = std::make_unique<Cast_spell_command>(2);
    key_bindings['4'] = std::make_unique<Cast_spell_command>(3);
    key_bindings['5'] = std::make_unique<Cast_spell_command>(4);
    key_bindings['c'] = std::make_unique<Save_command>();
    key_bindings['j'] = std::make_unique<Load_command>();
    key_bindings['h'] = std::make_unique<Heal_command>();
    key_bindings['n'] = std::make_unique<Learn_spell_command>();
    key_bindings['z'] = std::make_unique<Quit_command>();
}

bool Input_handler::load_bindings_from_file(const std::string& file_name) {
    std::ifstream file(file_name);
    if (!file.is_open()) {
        std::cout << "Файл настроек не найден, используются стандартные настройки" << std::endl;
        return false;
    }
    
    std::map<char, std::string> temp_bindings;
    std::map<std::string, char> command_to_key;
    std::string line;
    int line_num = 0;
    
    while (std::getline(file, line)) {
        line_num++;
        std::istringstream iss(line);
        char key;
        std::string command_name;
        
        if (!(iss >> key >> command_name)) {
            std::cout << "Ошибка в строке " << line_num << ": некорректный формат" << std::endl;
            setup_default_bindings();
            return false;
        }
        
        if (temp_bindings.find(key) != temp_bindings.end()) {
            std::cout << "Ошибка: клавиша '" << key << "' назначена на несколько команд" << std::endl;
            setup_default_bindings();
            return false;
        }
        
        if (command_to_key.find(command_name) != command_to_key.end()) {
            std::cout << "Ошибка: команда '" << command_name << "' назначена на несколько клавиш" << std::endl;
            setup_default_bindings();
            return false;
        }
        
        temp_bindings[key] = command_name;
        command_to_key[command_name] = key;
    }
    
    std::vector<std::string> required_commands = {
        "move_up", "move_down", "move_left", "move_right", 
        "attack", "switch_mode", "cast_spell_1", "cast_spell_2",
        "cast_spell_3", "cast_spell_4", "cast_spell_5", "save", "load",
        "heal", "learn_spell", "quit"
    };
    
    for (const auto& req_cmd : required_commands) {
        if (command_to_key.find(req_cmd) == command_to_key.end()) {
            std::cout << "Ошибка: отсутствует команда '" << req_cmd << "'" << std::endl;
            setup_default_bindings();
            return false;
        }
    }
    
    key_bindings.clear();
    
    for (const auto& binding : temp_bindings) {
        char key = binding.first;
        const std::string& cmd_name = binding.second;
        
        if (cmd_name == "move_up") {
            key_bindings[key] = std::make_unique<Move_command>(0, -1);
        } else if (cmd_name == "move_down") {
            key_bindings[key] = std::make_unique<Move_command>(0, 1);
        } else if (cmd_name == "move_left") {
            key_bindings[key] = std::make_unique<Move_command>(-1, 0);
        } else if (cmd_name == "move_right") {
            key_bindings[key] = std::make_unique<Move_command>(1, 0);
        } else if (cmd_name == "attack") {
            key_bindings[key] = std::make_unique<Attack_command>();
        } else if (cmd_name == "switch_mode") {
            key_bindings[key] = std::make_unique<Switch_mode_command>();
        } else if (cmd_name == "cast_spell_1") {
            key_bindings[key] = std::make_unique<Cast_spell_command>(0);
        } else if (cmd_name == "cast_spell_2") {
            key_bindings[key] = std::make_unique<Cast_spell_command>(1);
        } else if (cmd_name == "cast_spell_3") {
            key_bindings[key] = std::make_unique<Cast_spell_command>(2);
        } else if (cmd_name == "cast_spell_4") {
            key_bindings[key] = std::make_unique<Cast_spell_command>(3);
        } else if (cmd_name == "cast_spell_5") {
            key_bindings[key] = std::make_unique<Cast_spell_command>(4);
        } else if (cmd_name == "save") {
            key_bindings[key] = std::make_unique<Save_command>();
        } else if (cmd_name == "load") {
            key_bindings[key] = std::make_unique<Load_command>();
        } else if (cmd_name == "heal") {
            key_bindings[key] = std::make_unique<Heal_command>();
        } else if (cmd_name == "learn_spell"){
            key_bindings[key] = std::make_unique<Learn_spell_command>();
        } else if (cmd_name == "quit") {
            key_bindings[key] = std::make_unique<Quit_command>();
        } else {
            std::cout << "Неизвестная команда: " << cmd_name << std::endl;
            setup_default_bindings();
            return false;
        }
    }
    std::cout << "Настройки управления успешно загружены из файла: " << file_name << std::endl;
    return true;
}

void Input_handler::set_binding(char key, std::unique_ptr<Command> command) {
    key_bindings[key] = std::move(command);
}

char Input_handler::get_key_for_command(const std::string& command_name) const {
    for (const auto& binding : key_bindings) {
        if (binding.second->get_name() == command_name) {
            return binding.first;
        }
    }
    return '\0';
}

Console_input_handler::Console_input_handler() : Input_handler() {}

std::unique_ptr<Command> Console_input_handler::get_next_command() {
    char input;
    std::cin >> input;
    
    auto it = key_bindings.find(input);
    if (it != key_bindings.end()) {
        return it->second->clone();
    }
    
    return nullptr;
}