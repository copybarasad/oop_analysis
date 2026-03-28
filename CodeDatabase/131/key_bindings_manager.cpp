#include "key_bindings_manager.h"
#include <iostream>
#include <sstream>
#include <algorithm>

namespace rpg {
    KeyBindingsManager::KeyBindingsManager() {
        InitializeRequiredCommands();
        SetDefaultBindings();
    }

    void KeyBindingsManager::InitializeRequiredCommands() {
        required_commands_ = {
            CommandType::kMoveUp,
            CommandType::kMoveDown,
            CommandType::kMoveLeft,
            CommandType::kMoveRight,
            CommandType::kAttack,
            CommandType::kSwitchMode,
            CommandType::kQuit
        };
    }

    void KeyBindingsManager::SetDefaultBindings() {
        bindings_.clear();
        bindings_['w'] = CommandType::kMoveUp;
        bindings_['s'] = CommandType::kMoveDown;
        bindings_['a'] = CommandType::kMoveLeft;
        bindings_['d'] = CommandType::kMoveRight;
        bindings_['e'] = CommandType::kAttack;
        bindings_['c'] = CommandType::kSwitchMode;
        bindings_['b'] = CommandType::kBuySpell;
        bindings_['0'] = CommandType::kCastSpell0;
        bindings_['1'] = CommandType::kCastSpell1;
        bindings_['2'] = CommandType::kCastSpell2;
        bindings_['3'] = CommandType::kCastSpell3;
        bindings_['4'] = CommandType::kCastSpell4;
        bindings_['v'] = CommandType::kSaveGame;
        bindings_['q'] = CommandType::kQuit;
    }

    bool KeyBindingsManager::LoadFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Не удалось открыть файл конфигурации: " << filename << "\n";
            std::cerr << "Используются настройки по умолчанию.\n";
            SetDefaultBindings();
            return false;
        }

        std::map<char, CommandType> temp_bindings;
        std::string line;
        int line_number = 0;

        while (std::getline(file, line)) {
            line_number++;
            
            if (line.empty() || line[0] == '#' || line[0] == ';') {
                continue;
            }

            size_t equals_pos = line.find('=');
            if (equals_pos == std::string::npos) {
                std::cerr << "Ошибка в строке " << line_number << ": неверный формат\n";
                continue;
            }

            std::string command_str = line.substr(0, equals_pos);
            std::string key_str = line.substr(equals_pos + 1);

            command_str.erase(std::remove_if(command_str.begin(), command_str.end(), ::isspace), command_str.end());
            key_str.erase(std::remove_if(key_str.begin(), key_str.end(), ::isspace), key_str.end());

            if (key_str.empty()) {
                std::cerr << "Ошибка в строке " << line_number << ": пустая клавиша\n";
                continue;
            }

            char key = key_str[0];
            CommandType cmd_type = StringToCommandType(command_str);

            if (cmd_type == CommandType::kInvalid) {
                std::cerr << "Ошибка в строке " << line_number << ": неизвестная команда '" << command_str << "'\n";
                continue;
            }

            temp_bindings[key] = cmd_type;
        }

        file.close();

        auto old_bindings = bindings_;
        bindings_ = temp_bindings;

        if (!ValidateBindings()) {
            std::cerr << "Конфигурация клавиш некорректна. Используются настройки по умолчанию.\n";
            SetDefaultBindings();
            return false;
        }

        std::cout << "Настройки управления успешно загружены из файла: " << filename << "\n";
        return true;
    }

    bool KeyBindingsManager::ValidateBindings() const {
        if (HasDuplicateCommands()) {
            std::cerr << "Ошибка валидации: одна команда назначена на несколько клавиш\n";
            return false;
        }

        if (!HasAllRequiredCommands()) {
            std::cerr << "Ошибка валидации: отсутствуют обязательные команды\n";
            return false;
        }

        return true;
    }

    bool KeyBindingsManager::HasDuplicateCommands() const {
        std::set<CommandType> seen_commands;
        
        for (const auto& [key, cmd_type] : bindings_) {
            if (cmd_type == CommandType::kInvalid) {
                continue;
            }
            
            if (seen_commands.count(cmd_type) > 0) {
                return true;
            }
            seen_commands.insert(cmd_type);
        }
        
        return false;
    }

    bool KeyBindingsManager::HasAllRequiredCommands() const {
        std::set<CommandType> present_commands;
        
        for (const auto& [key, cmd_type] : bindings_) {
            present_commands.insert(cmd_type);
        }
        
        for (const auto& required_cmd : required_commands_) {
            if (present_commands.count(required_cmd) == 0) {
                return false;
            }
        }
        
        return true;
    }

    CommandType KeyBindingsManager::GetCommandType(char key) const {
        auto it = bindings_.find(key);
        if (it != bindings_.end()) {
            return it->second;
        }
        return CommandType::kInvalid;
    }

    CommandType KeyBindingsManager::StringToCommandType(const std::string& str) const {
        if (str == "move_up") return CommandType::kMoveUp;
        if (str == "move_down") return CommandType::kMoveDown;
        if (str == "move_left") return CommandType::kMoveLeft;
        if (str == "move_right") return CommandType::kMoveRight;
        if (str == "attack") return CommandType::kAttack;
        if (str == "switch_mode") return CommandType::kSwitchMode;
        if (str == "buy_spell") return CommandType::kBuySpell;
        if (str == "cast_spell_0") return CommandType::kCastSpell0;
        if (str == "cast_spell_1") return CommandType::kCastSpell1;
        if (str == "cast_spell_2") return CommandType::kCastSpell2;
        if (str == "cast_spell_3") return CommandType::kCastSpell3;
        if (str == "cast_spell_4") return CommandType::kCastSpell4;
        if (str == "save_game") return CommandType::kSaveGame;
        if (str == "quit") return CommandType::kQuit;
        
        return CommandType::kInvalid;
    }
}

