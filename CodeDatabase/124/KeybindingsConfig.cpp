#include "KeybindingsConfig.h"
#include "json.hpp"
#include <fstream>
#include <iostream>

using json = nlohmann::json;

KeybindingsConfig::KeybindingsConfig() {
    if (!loadFromFile()) {
        std::cout << "Не удалось загрузить конфиг, устанавливаю значения по умолчанию\n";
        setDefaultKeybindings();
        saveToFile();
    }
}

bool KeybindingsConfig::loadFromFile() {
    std::ifstream file(configFile);
    if (!file.is_open()) {
        return false;
    }
    
    try {
        json j;
        file >> j;
        
        keyToCommand.clear();
        commandConfigs.clear();
         
        if (j.contains("keybindings") && j["keybindings"].is_object()) {
            for (auto& [keyStr, command] : j["keybindings"].items()) {
                if (keyStr.length() == 1) {
                    keyToCommand[keyStr[0]] = command;
                }
            }
        }
         
        if (j.contains("commands") && j["commands"].is_object()) {
            for (auto& [command, config] : j["commands"].items()) {
                CommandConfig cmdConfig;
                cmdConfig.description = config.value("description", "");
                cmdConfig.consumesTurn = config.value("consumes_turn", true);
                commandConfigs[command] = cmdConfig;
            }
        }
        
        return validateConfig();
        
    } catch (const std::exception& e) {
        std::cout << "Ошибка загрузки конфига: " << e.what() << "\n";
        return false;
    }
}

bool KeybindingsConfig::saveToFile() const {
    std::ofstream file(configFile);
    if (!file.is_open()) {
        return false;
    }
    
    json j;
     
    json keybindings;
    for (const auto& [key, command] : keyToCommand) {
        std::string keyStr(1, key);
        keybindings[keyStr] = command;
    }
    j["keybindings"] = keybindings;
     
    json commands;
    for (const auto& [command, config] : commandConfigs) {
        json cmdJson;
        cmdJson["description"] = config.description;
        cmdJson["consumes_turn"] = config.consumesTurn;
        commands[command] = cmdJson;
    }
    j["commands"] = commands;
    
    file << j.dump(4);
    return true;
}

void KeybindingsConfig::setDefaultKeybindings() { 
    keyToCommand.clear();
    commandConfigs.clear();
     
    keyToCommand['w'] = "move_up";
    keyToCommand['s'] = "move_down"; 
    keyToCommand['a'] = "move_left";
    keyToCommand['d'] = "move_right";
    keyToCommand['x'] = "attack";
    keyToCommand['t'] = "switch_attack";
    keyToCommand['c'] = "cast_spell";
    keyToCommand['q'] = "prev_spell";
    keyToCommand['e'] = "next_spell";
    keyToCommand['l'] = "save_game";
    keyToCommand['o'] = "load_game";
    keyToCommand['b'] = "buy_spell";
    keyToCommand['m'] = "open_shop";
     
    commandConfigs["move_up"] = {"Движение вверх", true};
    commandConfigs["move_down"] = {"Движение вниз", true};
    commandConfigs["move_left"] = {"Движение влево", true};
    commandConfigs["move_right"] = {"Движение вправо", true};
    commandConfigs["attack"] = {"Атака врага", true};
    commandConfigs["switch_attack"] = {"Смена типа атаки", false};
    commandConfigs["cast_spell"] = {"Применить заклинание", true};
    commandConfigs["prev_spell"] = {"Предыдущее заклинание", false};
    commandConfigs["next_spell"] = {"Следующее заклинание", false};
    commandConfigs["save_game"] = {"Сохранить игру", false};
    commandConfigs["load_game"] = {"Загрузить игру", false};
    commandConfigs["buy_spell"] = {"Купить заклинание", true};
    commandConfigs["open_shop"] = {"Открыть магазин", false};
}

std::string KeybindingsConfig::getCommand(char key) const {
    auto it = keyToCommand.find(key);
    return (it != keyToCommand.end()) ? it->second : "";
}

CommandConfig KeybindingsConfig::getCommandConfig(const std::string& command) const {
    auto it = commandConfigs.find(command);
    return (it != commandConfigs.end()) ? it->second : CommandConfig{"", true};
}

std::vector<char> KeybindingsConfig::getKeysForCommand(const std::string& command) const {
    std::vector<char> keys;
    for (const auto& [key, cmd] : keyToCommand) {
        if (cmd == command) {
            keys.push_back(key);
        }
    }
    return keys;
}

bool KeybindingsConfig::validateConfig() const { 
    for (const auto& [key, command] : keyToCommand) {
        if (commandConfigs.find(command) == commandConfigs.end()) {
            std::cout << "Не найдена конфигурация для команды: " << command << "\n";
            return false;
        }
    }
     
    std::unordered_map<char, int> keyCount;
    for (const auto& [key, command] : keyToCommand) {
        keyCount[key]++;
        if (keyCount[key] > 1) {
            std::cout << "Ошибка: Клавиша '" << key << "' назначена на несколько команд!\n";
            return false;
        }
    }
     
    std::unordered_map<std::string, std::vector<char>> commandToKeys;
    for (const auto& [key, command] : keyToCommand) {
        commandToKeys[command].push_back(key);
    }
    
    for (const auto& [command, keys] : commandToKeys) {
        if (keys.size() > 1) {
            std::cout << "Ошибка: Команда '" << command 
                      << "' назначена на несколько клавиш: ";
            for (char k : keys) std::cout << "'" << k << "' ";
            std::cout << "\n";
            return false;
        }
    }
    
    return true;
}

void KeybindingsConfig::printCurrentBindings() const {
    std::cout << "\n=== ТЕКУЩИЕ НАСТРОЙКИ УПРАВЛЕНИЯ ===\n";
    for (const auto& [key, command] : keyToCommand) {
        auto config = getCommandConfig(command);
        std::cout << "'" << key << "' -> " << command << " - " << config.description;
        std::cout << " [" << (config.consumesTurn ? "ход" : "нет хода") << "]\n";
    }
    std::cout << "===================================\n";
} 