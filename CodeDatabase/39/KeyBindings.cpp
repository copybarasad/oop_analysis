#include "KeyBindings.h"
#include <iostream>
#include <fstream>
#include <cctype>

KeyBindings::KeyBindings() {
    SetDefault();
}

void KeyBindings::LoadFromFile(const std::string& filename) {
    std::ifstream in(filename);
    if (!in) {
        std::cout << "Cannot open keybindings file, using default.\n";
        SetDefault();
        return;
    }

    std::map<char, CommandType> new_map;
    std::map<CommandType, char> reverse_map;

    std::string line;
    while (std::getline(in, line)) {
        if (line.empty()) continue;

        auto pos = line.find('=');
        if (pos == std::string::npos) {
            std::cout << "Invalid line in keybindings, using default.\n";
            SetDefault();
            return;
        }

        std::string key_str = line.substr(0, pos);
        std::string cmd_str = line.substr(pos + 1);

        if (key_str.size() != 1) {
            std::cout << "Invalid key in keybindings, using default.\n";
            SetDefault();
            return;
        }

        char key = static_cast<char>(std::toupper(static_cast<unsigned char>(key_str[0])));
        auto parsed = ParseCommand(cmd_str);
        if (!parsed) {
            std::cout << "Unknown command in keybindings, using default.\n";
            SetDefault();
            return;
        }
        CommandType cmd = *parsed;

        if (new_map.count(key)) {
            std::cout << "Duplicate key in keybindings, using default.\n";
            SetDefault();
            return;
        }
        if (reverse_map.count(cmd)) {
            std::cout << "Command bound to multiple keys, using default.\n";
            SetDefault();
            return;
        }

        new_map[key] = cmd;
        reverse_map[cmd] = key;
    }

    for (CommandType required : AllRequiredCommands()) {
        if (!reverse_map.count(required)) {
            std::cout << "Missing command in keybindings, using default.\n";
            SetDefault();
            return;
        }
    }

    key_to_command_ = std::move(new_map);
    std::cout << "Custom keybindings loaded.\n";
}

CommandType KeyBindings::GetCommandForKey(char c) const {
    char key = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
    auto it = key_to_command_.find(key);
    if (it != key_to_command_.end()) {
        return it->second;
    }
    return CommandType::None;
}

void KeyBindings::SetDefault() {
    key_to_command_.clear();
    key_to_command_['W'] = CommandType::MoveUp;
    key_to_command_['S'] = CommandType::MoveDown;
    key_to_command_['A'] = CommandType::MoveLeft;
    key_to_command_['D'] = CommandType::MoveRight;
    key_to_command_['M'] = CommandType::SwitchMode;
    key_to_command_['F'] = CommandType::RangedAttack;
    key_to_command_['C'] = CommandType::CastSpell;
    key_to_command_['K'] = CommandType::Save;
    key_to_command_['L'] = CommandType::Load;
    key_to_command_['Q'] = CommandType::Quit;
}

std::optional<CommandType> KeyBindings::ParseCommand(const std::string& s_raw) {
    std::string s;
    for (char ch : s_raw) {
        if (!std::isspace(static_cast<unsigned char>(ch)))
            s.push_back(static_cast<char>(std::toupper(static_cast<unsigned char>(ch))));
    }

    if (s == "MOVEUP")        return CommandType::MoveUp;
    if (s == "MOVEDOWN")      return CommandType::MoveDown;
    if (s == "MOVELEFT")      return CommandType::MoveLeft;
    if (s == "MOVERIGHT")     return CommandType::MoveRight;
    if (s == "SWITCHMODE")    return CommandType::SwitchMode;
    if (s == "RANGEDATTACK")  return CommandType::RangedAttack;
    if (s == "CASTSPELL")     return CommandType::CastSpell;
    if (s == "SAVE")          return CommandType::Save;
    if (s == "LOAD")          return CommandType::Load;
    if (s == "QUIT")          return CommandType::Quit;

    return std::nullopt;
}

std::vector<CommandType> KeyBindings::AllRequiredCommands() {
    return {
        CommandType::MoveUp,
        CommandType::MoveDown,
        CommandType::MoveLeft,
        CommandType::MoveRight,
        CommandType::SwitchMode,
        CommandType::RangedAttack,
        CommandType::CastSpell,
        CommandType::Save,
        CommandType::Load,
        CommandType::Quit
    };
}
