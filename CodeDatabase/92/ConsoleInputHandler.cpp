#include "ConsoleInputHandler.h"
#include <iostream>
#include <fstream>
#include <set>
#include "nlohmann/json.hpp"

using json = nlohmann::json;

ConsoleInputHandler::ConsoleInputHandler() {
    load_config();
}

void ConsoleInputHandler::load_config() {
    // Управление по умолчанию
    std::map<char, CommandType> defaults = {
        {'w', CommandType::MoveUp},
        {'s', CommandType::MoveDown},
        {'a', CommandType::MoveLeft},
        {'d', CommandType::MoveRight},
        {'e', CommandType::SwitchWeapon},
        {'f', CommandType::CastSpell},
        {'m', CommandType::OpenShop},
        {'k', CommandType::SaveGame},
        {'q', CommandType::Quit}
    };

    std::ifstream f("controls.json");
    if (!f.is_open()) {
        key_map = defaults;
        return;
    }

    try {
        json j;
        f >> j;
        std::map<char, CommandType> loaded_map;
        std::set<char> used_keys;
        std::set<int> used_cmds;

        std::map<std::string, CommandType> str_to_cmd = {
            {"UP", CommandType::MoveUp}, {"DOWN", CommandType::MoveDown},
            {"LEFT", CommandType::MoveLeft}, {"RIGHT", CommandType::MoveRight},
            {"SWITCH", CommandType::SwitchWeapon}, {"SPELL", CommandType::CastSpell},
            {"SHOP", CommandType::OpenShop}, {"SAVE", CommandType::SaveGame},
            {"QUIT", CommandType::Quit}
        };

        for (auto const& [key, val] : j.items()) {
            if (str_to_cmd.find(key) == str_to_cmd.end()) throw std::exception();
            
            std::string key_str = val;
            if (key_str.length() != 1) throw std::exception();
            char k = key_str[0];

            if (used_keys.count(k)) throw std::exception();
            
            loaded_map[k] = str_to_cmd[key];
            used_keys.insert(k);
            used_cmds.insert((int)str_to_cmd[key]);
        }

        if (used_cmds.size() != str_to_cmd.size()) throw std::exception();

        key_map = loaded_map;
    } catch (...) {
        std::cout << "Ошибка конфига. Используем настройки по умолчанию.\n";
        key_map = defaults;
    }

    std::string keys_str;
    for (auto const& [key, val] : key_map) {
        keys_str += key;
        keys_str += "/";
    }
    if (!keys_str.empty()) keys_str.pop_back();

    input_prompt = "Выберите действие (" + keys_str + "): ";
}

Command ConsoleInputHandler::get_command() {
    std::cout << input_prompt;
    std::string input;
    std::getline(std::cin, input);
    if (input.empty()) return {CommandType::None};
    char c = input[0];
    if (key_map.find(c) != key_map.end()) {
        return {key_map[c]};
    }
    std::cout << "Такого действия не существует. Попробуйте ещё раз.\n";
    return {CommandType::None};
}

int ConsoleInputHandler::get_int(const std::string& prompt) {
    bool valid_int = false;
    int result = 0;

    while (!valid_int) {
        std::cout << prompt;
        std::string s;
        std::getline(std::cin, s);

        try {
            result = std::stoi(s);
            valid_int = true;
        } catch(...) {
            std::cout << "Неверное число.\n";
        }
    }

    return result;
}


std::string ConsoleInputHandler::get_string(const std::string& prompt) {
    std::cout << prompt;
    std::string s;
    std::getline(std::cin, s);
    return s;
}
