#include "input_handler.h"
#include <fstream>
#include <sstream>
#include <set>
#include <algorithm>
#include <cstdio>
#include <cctype>

CommandType stringToCommand(const std::string& cmd) {
    if (cmd == "MoveUp") return CommandType::MoveUp;
    if (cmd == "MoveDown") return CommandType::MoveDown;
    if (cmd == "MoveLeft") return CommandType::MoveLeft;
    if (cmd == "MoveRight") return CommandType::MoveRight;
    if (cmd == "ChangeStyle") return CommandType::ChangeStyle;
    if (cmd == "Skip") return CommandType::Skip;
    if (cmd == "Quit") return CommandType::Quit;
    if (cmd == "Save") return CommandType::Save;
    return CommandType::Invalid;
}

void KeyBindings::setDefault() {
    bindings.clear();
    bindings['w'] = CommandType::MoveUp;
    bindings['s'] = CommandType::MoveDown;
    bindings['a'] = CommandType::MoveLeft;
    bindings['d'] = CommandType::MoveRight;
    bindings['c'] = CommandType::ChangeStyle;
    bindings['x'] = CommandType::Skip;
    bindings['q'] = CommandType::Quit;
    bindings['z'] = CommandType::Save;
}

bool KeyBindings::load(const std::string& file) {
    std::ifstream ifs(file);
    if (!ifs.is_open()) return false;

    std::map<char, CommandType> temp_bindings;
    std::set<char> used_keys;
    std::set<CommandType> used_cmds;

    std::string line;
    while (std::getline(ifs, line)) {
        if (line.empty() || line[0] == '#') continue;

        size_t pos = line.find('=');
        if (pos == std::string::npos || pos == 0 || pos == line.length() - 1) {
            return false;
        }

        std::string cmd_str = line.substr(0, pos);
        cmd_str.erase(std::remove_if(cmd_str.begin(), cmd_str.end(), ::isspace), cmd_str.end());
        
        char key = line[pos + 1];
        CommandType type = stringToCommand(cmd_str);

        if (type == CommandType::Invalid || used_keys.count(key) || used_cmds.count(type)) {
            return false; 
        }

        temp_bindings[key] = type;
        used_keys.insert(key);
        used_cmds.insert(type);
    }

    bindings = std::move(temp_bindings);
    return true;
}

CommandType KeyBindings::get(char key) const {
    auto it = bindings.find(key);
    return it == bindings.end() ? CommandType::Invalid : it->second;
}

ConsoleInput::ConsoleInput() {
    if (!bindings.load("controls.cfg")) {
        std::cerr << "Предупреждение: controls.cfg не найден или некорректен. Установлено управление по умолчанию.\n";
        bindings.setDefault();
    }
}

Command ConsoleInput::getCommand() {
    std::string input;
    std::getline(std::cin >> std::ws, input);

    Command cmd;
    if (input.empty()) return cmd;

    if (input.length() == 1 && !std::isdigit(static_cast<unsigned char>(input[0]))) {
        cmd.type = bindings.get(input[0]);
        return cmd;
    }

    int x, y;
    if (std::sscanf(input.c_str(), "%d.%d", &x, &y) == 2) {
        cmd.type = CommandType::AttackCell;
        cmd.x = x;
        cmd.y = y;
        return cmd;
    } 
    
    int n;
    char buf[64];
    if (std::sscanf(input.c_str(), "%d %63s", &n, buf) == 2) {
        int cx, cy;
        if (std::sscanf(buf, "%d.%d", &cx, &cy) == 2) {
            cmd.type = CommandType::CastSpell;
            cmd.spell_index = n;
            cmd.x = cx;
            cmd.y = cy;
            return cmd;
        }
    }

    return cmd;
}