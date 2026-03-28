#include "input_handler.h"
#include "move_command.h"
#include "attack_command.h"
#include "switch_weapon_command.h"
#include "cast_spell_command.h"
#include "buy_spell_command.h"
#include "save_command.h"
#include "load_command.h"
#include "quit_command.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <set>
#include <vector>
#include <algorithm>

InputHandler::InputHandler() {
    register_commands();
    set_default_controls();
}

void InputHandler::register_commands() {
    command_registry_["move_up"] = std::make_shared<MoveCommand>(0, -1);
    command_registry_["move_down"] = std::make_shared<MoveCommand>(0, 1);
    command_registry_["move_left"] = std::make_shared<MoveCommand>(-1, 0);
    command_registry_["move_right"] = std::make_shared<MoveCommand>(1, 0);
    command_registry_["attack"] = std::make_shared<AttackCommand>();
    command_registry_["weapon_melee"] = std::make_shared<SwitchWeaponCommand>(true);
    command_registry_["weapon_range"] = std::make_shared<SwitchWeaponCommand>(false);
    command_registry_["cast_spell"] = std::make_shared<CastSpellCommand>();
    command_registry_["buy_spell"] = std::make_shared<BuySpellCommand>();
    command_registry_["save"] = std::make_shared<SaveCommand>();
    command_registry_["load"] = std::make_shared<LoadCommand>();
    command_registry_["quit"] = std::make_shared<QuitCommand>();
}

void InputHandler::set_default_controls() {
    key_bindings_.clear();
    key_bindings_['w'] = "move_up";
    key_bindings_['s'] = "move_down";
    key_bindings_['a'] = "move_left";
    key_bindings_['d'] = "move_right";
    key_bindings_['f'] = "attack";
    key_bindings_['1'] = "weapon_melee";
    key_bindings_['2'] = "weapon_range";
    key_bindings_['c'] = "cast_spell";
    key_bindings_['b'] = "buy_spell";
    key_bindings_['k'] = "save";
    key_bindings_['l'] = "load";
    key_bindings_['q'] = "quit";
}

void InputHandler::save_config(const std::string& filename) {
    std::ofstream file(filename);
    if (file.is_open()) {
        std::vector<std::string> logical_order = {
            "move_up", "move_left", "move_down", "move_right",
            "attack", "cast_spell", "buy_spell",
            "weapon_melee", "weapon_range",
            "save", "load", "quit"
        };
        for (const auto& cmd_name : logical_order) {
            char key = find_key_for_command(cmd_name);
            if (key != '?') {
                file << (char)std::tolower(key) << " " << cmd_name << "\n";
            }
        }
        std::cout << "Создан файл конфигурации управления: " << filename << std::endl;
    }
}

void InputHandler::load_config(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "Файл управления не найден. Создаю дефолтный..." << std::endl;
        set_default_controls();
        save_config(filename); 
        return;
    }
    std::map<char, std::string> temp_bindings;
    std::set<char> used_keys;
    std::set<std::string> found_commands;
    std::string line;
    bool error = false;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        char key;
        std::string cmd;
        if (ss >> key >> cmd) {
            key = std::tolower(key);
            if (command_registry_.find(cmd) == command_registry_.end() || used_keys.count(key)) {
                error = true; break;
            }
            if (found_commands.count(cmd)) {
                 error = true; break;
            }
            temp_bindings[key] = cmd;
            used_keys.insert(key);
            found_commands.insert(cmd);
        }
    }
    if (found_commands.size() < command_registry_.size()) error = true;
    if (error) {
        std::cout << "Ошибка в конфиге! Сброс на стандартное." << std::endl;
        set_default_controls();
    } else {
        key_bindings_ = temp_bindings;
        std::cout << "Управление загружено из файла!" << std::endl;
    }
}

std::shared_ptr<Command> InputHandler::handle_input() {
    std::cout << "\nВведите команду: ";
    std::string input;
    if (!std::getline(std::cin, input) || input.empty()) return nullptr;
    char key = std::tolower(input[0]);
    if (key_bindings_.count(key)) return command_registry_[key_bindings_[key]];
    std::cout << "Неизвестная команда!" << std::endl;
    return nullptr;
}

char InputHandler::find_key_for_command(const std::string& cmd) const {
    for (const auto& pair : key_bindings_) {
        if (pair.second == cmd) return std::toupper(pair.first);
    }
    return '?';
}

std::string InputHandler::get_controls_help_string() const {
    std::stringstream ss;
    ss << "Управление: "
       << find_key_for_command("move_up") << "-" 
       << find_key_for_command("move_left") << "-"
       << find_key_for_command("move_down") << "-"
       << find_key_for_command("move_right") << " движение, "
       << find_key_for_command("weapon_melee") << "-" 
       << find_key_for_command("weapon_range") << " оружие, "
       << find_key_for_command("quit") << " выход, "
       << find_key_for_command("attack") << " атака, "
       << find_key_for_command("cast_spell") << " магия, "
       << find_key_for_command("buy_spell") << " купить, "
       << find_key_for_command("save") << " сохр, "
       << find_key_for_command("load") << " загр";
    return ss.str();
}