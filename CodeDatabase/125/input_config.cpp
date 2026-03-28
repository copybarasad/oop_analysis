#include "input_config.h"
#include <fstream>
#include <iostream>
#include <cctype>
#include "include/json.hpp"

using json = nlohmann::json;

InputConfig& InputConfig::GetInstance() {
    static InputConfig instance;
    return instance;
}

void InputConfig::SetInstance(const InputConfig& config) {
    InputConfig& instance = GetInstance();
    instance.key_to_command_ = config.key_to_command_;
    instance.command_to_key_ = config.command_to_key_;
    instance.valid_ = config.valid_;
}

InputConfig::InputConfig() : valid_(false) {
    SetDefaultConfig();
}

bool InputConfig::LoadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "Не удалось открыть файл конфигурации: " << filename 
                  << ". Используются настройки по умолчанию.\n";
        return false;
    }
    
    try {
        json config;
        file >> config;
        
        if (!config.contains("version") || !config.contains("controls")) {
            std::cout << "Некорректный формат файла конфигурации.\n";
            return false;
        }
        
        std::unordered_map<char, CommandType> temp_key_to_command;
        std::unordered_map<CommandType, char> temp_command_to_key;
        std::unordered_map<char, std::vector<std::string>> key_assignments; 

        json controls = config["controls"];
        
        std::unordered_map<std::string, CommandType> string_to_command = {
            {"move_up", CommandType::MoveUp},
            {"move_down", CommandType::MoveDown},
            {"move_left", CommandType::MoveLeft},
            {"move_right", CommandType::MoveRight},
            {"attack", CommandType::Attack},
            {"switch_mode", CommandType::SwitchMode},
            {"cast_spell_1", CommandType::CastSpell1},
            {"cast_spell_2", CommandType::CastSpell2},
            {"cast_spell_3", CommandType::CastSpell3},
            {"open_shop", CommandType::OpenShop},
            {"save_game", CommandType::SaveGame}
        };
        
        for (const auto& [command_str, key_str] : controls.items()) {
            if (!string_to_command.count(command_str)) {
                std::cout << "Неизвестная команда в конфиге: " << command_str << "\n";
                continue;
            }
            
            if (key_str.empty() || key_str.size() != 1) {
                std::cout << "Некорректная клавиша для команды " << command_str << ": " << key_str << "\n";
                continue;
            }
            
            char key = key_str.get<std::string>()[0];
            CommandType command = string_to_command[command_str];
            
            if (temp_key_to_command.count(key)) {
                std::cout << "ОШИБКА: клавиша '" << key 
                          << "' уже назначена на команду. Конфликт: " 
                          << command_str << "\n";
                std::cout << "Используются настройки по умолчанию.\n";
                SetDefaultConfig();
                return false;
            }
            
            if (temp_command_to_key.count(command)) {
                std::cout << "ОШИБКА: команда '" << command_str 
                          << "' уже назначена на клавишу '" 
                          << temp_command_to_key[command] << "'\n";
                std::cout << "Используются настройки по умолчанию.\n";
                SetDefaultConfig();
                return false;
            }
            
            temp_key_to_command[key] = command;
            temp_command_to_key[command] = key;
            key_assignments[key].push_back(command_str);
        }
        
        bool all_commands_assigned = true;
        for (const auto& [command_str, command_type] : string_to_command) {
            if (!temp_command_to_key.count(command_type)) {
                std::cout << "ОШИБКА: команда '" << command_str << "' не назначена на клавишу\n";
                all_commands_assigned = false;
            }
        }
        
        if (!all_commands_assigned) {
            std::cout << "Используются настройки по умолчанию.\n";
            SetDefaultConfig();
            return false;
        }
        
        key_to_command_ = std::move(temp_key_to_command);
        command_to_key_ = std::move(temp_command_to_key);
        valid_ = true;
        
        std::cout << "Конфигурация управления загружена из файла: " << filename << "\n";
        return true;
        
    } catch (const json::exception& e) {
        std::cout << "Ошибка чтения JSON: " << e.what() << "\n";
        SetDefaultConfig();
        return false;
    }
}

CommandType InputConfig::GetCommandType(char input) const {
    auto it = key_to_command_.find(input);
    if (it != key_to_command_.end()) {
        return it->second;
    }
    return CommandType::Invalid;
}

char InputConfig::GetInputSymbol(CommandType command) const {
    auto it = command_to_key_.find(command);
    if (it != command_to_key_.end()) {
        return it->second;
    }
    return '\0';
}

void InputConfig::SetDefaultConfig() {
    key_to_command_.clear();
    command_to_key_.clear();
    
    key_to_command_ = {
        {'w', CommandType::MoveUp},
        {'s', CommandType::MoveDown},
        {'a', CommandType::MoveLeft},
        {'d', CommandType::MoveRight},
        {'f', CommandType::Attack},
        {'m', CommandType::SwitchMode},
        {'1', CommandType::CastSpell1},
        {'2', CommandType::CastSpell2},
        {'3', CommandType::CastSpell3},
        {'e', CommandType::OpenShop},
        {'c', CommandType::SaveGame}
    };
    
    for (const auto& [key, command] : key_to_command_) {
        command_to_key_[command] = key;
    }
    
    valid_ = true;
}

bool InputConfig::ValidateConfig() const {
    std::vector<CommandType> all_commands = {
        CommandType::MoveUp, CommandType::MoveDown, CommandType::MoveLeft, CommandType::MoveRight,
        CommandType::Attack, CommandType::SwitchMode,
        CommandType::CastSpell1, CommandType::CastSpell2, CommandType::CastSpell3,
        CommandType::OpenShop, CommandType::SaveGame
    };
    
    for (const auto& command : all_commands) {
        if (!command_to_key_.count(command)) {
            std::cout << "Ошибка: команда не назначена на клавишу.\n";
            return false;
        }
    }
    
    std::unordered_map<char, int> key_count;
    for (const auto& [key, _] : key_to_command_) {
        key_count[key]++;
        if (key_count[key] > 1) {
            std::cout << "Ошибка: клавиша '" << key << "' назначена на несколько команд.\n";
            return false;
        }
    }
    
    std::unordered_map<CommandType, int> command_count;
    for (const auto& [command, _] : command_to_key_) {
        command_count[command]++;
        if (command_count[command] > 1) {
            std::cout << "Ошибка: команда назначена на несколько клавиш.\n";
            return false;
        }
    }
    
    return true;
}

std::string InputConfig::GetHelpString() const {
    std::string help = "Управление:\n";
    
    std::unordered_map<CommandType, std::string> command_descriptions = {
        {CommandType::MoveUp, "Движение вверх"},
        {CommandType::MoveDown, "Движение вниз"},
        {CommandType::MoveLeft, "Движение влево"},
        {CommandType::MoveRight, "Движение вправо"},
        {CommandType::Attack, "Атака"},
        {CommandType::SwitchMode, "Смена режима боя"},
        {CommandType::CastSpell1, "Заклинание 1"},
        {CommandType::CastSpell2, "Заклинание 2"},
        {CommandType::CastSpell3, "Заклинание 3"},
        {CommandType::OpenShop, "Открыть магазин"},
        {CommandType::SaveGame, "Сохранить игру"}
    };
    
    for (const auto& [command, key] : command_to_key_) {
        if (command_descriptions.count(command)) {
            help += "  " + std::string(1, key) + " - " + command_descriptions[command] + "\n";
        }
    }
    
    return help;
}
