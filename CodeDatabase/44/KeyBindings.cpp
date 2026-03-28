#include "KeyBindings.h"
#include <fstream>
#include <iostream>
#include <set>
#include <algorithm>

KeyBindings::KeyBindings() {
    SetDefaults();
}

void KeyBindings::SetDefaults() {
    key_to_command_.clear();
    key_to_command_[kDefaultMoveUp] = CommandType::MOVE_UP;
    key_to_command_[kDefaultMoveDown] = CommandType::MOVE_DOWN;
    key_to_command_[kDefaultMoveLeft] = CommandType::MOVE_LEFT;
    key_to_command_[kDefaultMoveRight] = CommandType::MOVE_RIGHT;
    key_to_command_[kDefaultAttack] = CommandType::ATTACK;
    key_to_command_[kDefaultSwitchWeapon] = CommandType::SWITCH_WEAPON;
    key_to_command_[kDefaultUseSpell] = CommandType::USE_SPELL;
    key_to_command_[kDefaultViewSpells] = CommandType::VIEW_SPELLS;
    key_to_command_[kDefaultSaveGame] = CommandType::SAVE_GAME;
    key_to_command_[kDefaultLoadGame] = CommandType::LOAD_GAME;
    key_to_command_[kDefaultQuit] = CommandType::QUIT;
}

bool KeyBindings::ValidateBindings(const std::map<char, CommandType>& bindings) const {
    std::set<CommandType> required_commands = {
        CommandType::MOVE_UP,
        CommandType::MOVE_DOWN,
        CommandType::MOVE_LEFT,
        CommandType::MOVE_RIGHT,
        CommandType::ATTACK,
        CommandType::SWITCH_WEAPON,
        CommandType::USE_SPELL,
        CommandType::VIEW_SPELLS,
        CommandType::SAVE_GAME,
        CommandType::LOAD_GAME,
        CommandType::QUIT
    };

    std::set<CommandType> found_commands;
    std::set<char> used_keys;

    for (const auto& [key, cmd] : bindings) {
        if (used_keys.count(key) > 0) {
            std::cerr << "Ошибка: клавиша '" << key << "' назначена дважды\n";
            return false;
        }
        used_keys.insert(key);

        if (found_commands.count(cmd) > 0) {
            std::cerr << "Ошибка: команда назначена на несколько клавиш\n";
            return false;
        }
        found_commands.insert(cmd);
    }

    for (CommandType cmd : required_commands) {
        if (found_commands.count(cmd) == 0) {
            std::cerr << "Ошибка: отсутствует привязка для команды\n";
            return false;
        }
    }

    return true;
}

void KeyBindings::LoadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Не удалось открыть файл настроек: " << filename << ". Используются настройки по умолчанию.\n";
        SetDefaults();
        return;
    }

    std::map<char, CommandType> new_bindings;
    std::map<std::string, CommandType> command_names = {
        {"move_up", CommandType::MOVE_UP},
        {"move_down", CommandType::MOVE_DOWN},
        {"move_left", CommandType::MOVE_LEFT},
        {"move_right", CommandType::MOVE_RIGHT},
        {"attack", CommandType::ATTACK},
        {"switch_weapon", CommandType::SWITCH_WEAPON},
        {"use_spell", CommandType::USE_SPELL},
        {"view_spells", CommandType::VIEW_SPELLS},
        {"save_game", CommandType::SAVE_GAME},
        {"load_game", CommandType::LOAD_GAME},
        {"quit", CommandType::QUIT}
    };

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') {
            continue;
        }

        size_t eq_pos = line.find('=');
        if (eq_pos == std::string::npos || eq_pos + 1 >= line.size()) {
            continue;
        }

        std::string cmd_name = line.substr(0, eq_pos);
        char key = line[eq_pos + 1];

        cmd_name.erase(std::remove_if(cmd_name.begin(), cmd_name.end(), ::isspace), cmd_name.end());

        auto it = command_names.find(cmd_name);
        if (it != command_names.end()) {
            if (new_bindings.count(key) > 0) {
                std::cerr << "Ошибка: клавиша '" << key << "' уже назначена на другую команду\n";
                std::cerr << "Некорректные настройки. Используются настройки по умолчанию.\n";
                SetDefaults();
                return;
            }
            new_bindings[key] = it->second;
        }
    }

    file.close();

    if (ValidateBindings(new_bindings)) {
        key_to_command_ = new_bindings;
        std::cerr << "Настройки управления загружены из " << filename << "\n";
    } else {
        std::cerr << "Некорректные настройки. Используются настройки по умолчанию.\n";
        SetDefaults();
    }
}

CommandType KeyBindings::GetCommandType(char key) const {
    auto it = key_to_command_.find(key);
    if (it != key_to_command_.end()) {
        return it->second;
    }
    return CommandType::NONE;
}

std::pair<int, int> KeyBindings::GetMoveOffset(CommandType type) const {
    switch (type) {
        case CommandType::MOVE_UP:    return {0, -1};
        case CommandType::MOVE_DOWN:  return {0, 1};
        case CommandType::MOVE_LEFT:  return {-1, 0};
        case CommandType::MOVE_RIGHT: return {1, 0};
        default: return {0, 0};
    }
}

bool KeyBindings::IsMoveCommand(CommandType type) const {
    return type == CommandType::MOVE_UP ||
           type == CommandType::MOVE_DOWN ||
           type == CommandType::MOVE_LEFT ||
           type == CommandType::MOVE_RIGHT;
}

char KeyBindings::GetKeyForCommand(CommandType type) const {
    for (const auto& [key, cmd] : key_to_command_) {
        if (cmd == type) {
            return key;
        }
    }
    return '?';
}
