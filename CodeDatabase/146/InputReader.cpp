#include "InputReader.h"
#include "Command.h"
#include "FileHandler.h"
#include "SaveLoadException.h"
#include <iostream>
#include <set>
#include <conio.h>
InputReader::InputReader(const std::string& config_file) {
    load_config(config_file);
}

void InputReader::load_config(const std::string& filename) {
    try {
        FileHandler file_handler(filename, std::ios::in);
        std::iostream& file = file_handler.get_stream();

        key_map.clear();
        std::set<char> used_keys;
        std::set<std::string> used_commands;
        std::string line;

        while (std::getline(file, line)) {
            if (line.empty()) continue;
            size_t delimiter_pos = line.find('=');
            if (delimiter_pos == std::string::npos || delimiter_pos != 1) {
                std::cerr << "Config Error: Invalid line format '" << line << "'. Expected 'key=command'. Reverting to defaults.\n";
                set_defaults();
                return;
            }

            char key = line[0];
            std::string action = line.substr(delimiter_pos + 1); 

            if (!action.empty() && action.back() == '\r') {
                action.pop_back();
            }

            // 1. Валидация: Клавиша уже занята
            if (used_keys.count(key)) {
                std::cerr << "Config Error: Key '" << key << "' assigned twice. Reverting to defaults.\n";
                set_defaults();
                return;
            }

            // 2. Валидация: Команда уже назначена
            if (used_commands.count(action)) {
                std::cerr << "Config Error: Command '" << action << "' assigned to multiple keys. Reverting to defaults.\n";
                set_defaults();
                return;
            }

            used_keys.insert(key);
            used_commands.insert(action);

            if (action == "up")         key_map[key] = []() { return new MoveCommand("top"); };
            else if (action == "down")  key_map[key] = []() { return new MoveCommand("down"); };
            else if (action == "left")  key_map[key] = []() { return new MoveCommand("left"); };
            else if (action == "right") key_map[key] = []() { return new MoveCommand("right"); };
            else if (action == "spell") key_map[key] = []() { return new SpellCommand(); };
            else if (action == "save")  key_map[key] = []() { return new SaveCommand(); };
            else if (action == "exit")  key_map[key] = []() { return new ExitCommand(); };
            else {
                std::cerr << "Config Error: Unknown command '" << action << "'. Reverting to defaults.\n";
                set_defaults();
                return;
            }
        }

        if (key_map.empty()) {
            std::cerr << "Config file was empty.\n";
            set_defaults();
        }

    }
    catch (const FileOpenException& e) {
        std::cout << "Config file '" << filename << "' not found (" << e.what() << "). Using defaults.\n";
        set_defaults();
    }
    catch (const std::exception& e) {
        std::cerr << "Unexpected error while reading config: " << e.what() << "\n";
        set_defaults();
    }
}

void InputReader::set_defaults() {
    key_map.clear();
    key_map['w'] = []() { return new MoveCommand("top"); };
    key_map['s'] = []() { return new MoveCommand("down"); };
    key_map['a'] = []() { return new MoveCommand("left"); };
    key_map['d'] = []() { return new MoveCommand("right"); };
    key_map['e'] = []() { return new SpellCommand(); };
    key_map['q'] = []() { return new SaveCommand(); };
    key_map['x'] = []() { return new ExitCommand(); };
    std::cout << "Controls set to default (WASD, E, Q, X)\n";
}

Command* InputReader::get_command() {
    std::cout << "> ";
    char key = _getch();

    std::cout << key << "\n";

    auto it = key_map.find(key);
    if (it != key_map.end()) {
        return it->second();
    }

    return new EmptyCommand();
}
