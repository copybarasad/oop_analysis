#include "KeyBindingConfig.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>

/**
 * @file KeyBindingConfig.cpp
 * @brief Реализация конфигурации клавиш
 */

/**
 * @brief Конструктор с дефолтными настройками
 */
KeyBindingConfig::KeyBindingConfig() : isValid(true) {
    loadDefaults();
}

/**
 * @brief Загрузить дефолтные настройки
 */
void KeyBindingConfig::loadDefaults() {
    keyToCommand.clear();
    commandToKey.clear();
    
    // Движение
    keyToCommand["W"] = CommandType::MOVE_UP;
    keyToCommand["A"] = CommandType::MOVE_LEFT;
    keyToCommand["S"] = CommandType::MOVE_DOWN;
    keyToCommand["D"] = CommandType::MOVE_RIGHT;
    
    // Действия
    keyToCommand["C"] = CommandType::CAST_SPELL;
    keyToCommand["M"] = CommandType::SWITCH_MODE;
    
    // Информация
    keyToCommand["I"] = CommandType::SHOW_INFO;
    keyToCommand["H"] = CommandType::SHOW_HELP;
    
    // Управление игрой
    keyToCommand["Q"] = CommandType::QUIT;
    keyToCommand["SAVE"] = CommandType::SAVE_GAME;
    keyToCommand["LOAD"] = CommandType::LOAD_GAME;
    
    // Заполняем обратный маппинг
    for (const auto& [key, cmd] : keyToCommand) {
        commandToKey[cmd] = key;
    }
    
    isValid = validate();
}

/**
 * @brief Загрузить из файла
 * @param filepath Путь к файлу
 * @return true если успешно
 */
bool KeyBindingConfig::loadFromFile(const std::string& filepath) {
    std::ifstream file(filepath);
    
    if (!file.is_open()) {
        std::cerr << "[KeyBindingConfig] Failed to open: " << filepath << "\n";
        std::cerr << "[KeyBindingConfig] Using default keybindings.\n";
        loadDefaults();
        return false;
    }
    
    // Временные мапы
    std::unordered_map<std::string, CommandType> tempKeyToCommand;
    std::unordered_map<CommandType, std::string> tempCommandToKey;
    
    std::string line;
    while (std::getline(file, line)) {
        // Пропускаем комментарии и пустые строки
        if (line.empty() || line[0] == '#' || line[0] == '{' || line[0] == '}') {
            continue;
        }
        
        // Парсим строку формата: "command_name": "KEY"
        size_t colonPos = line.find(':');
        if (colonPos == std::string::npos) continue;
        
        std::string commandName = line.substr(0, colonPos);
        std::string key = line.substr(colonPos + 1);
        
        // Убираем кавычки, пробелы, запятые
        auto cleanString = [](std::string& s) {
            s.erase(std::remove_if(s.begin(), s.end(), 
                [](char c) { return c == '"' || c == '\'' || c == ',' || c == ' '; }), s.end());
        };
        
        cleanString(commandName);
        cleanString(key);
        
        if (commandName.empty() || key.empty()) continue;
        
        // Преобразуем в верхний регистр
        std::transform(key.begin(), key.end(), key.begin(), ::toupper);
        
        // Маппинг имён команд
        CommandType cmdType;
        if (commandName == "move_up") cmdType = CommandType::MOVE_UP;
        else if (commandName == "move_down") cmdType = CommandType::MOVE_DOWN;
        else if (commandName == "move_left") cmdType = CommandType::MOVE_LEFT;
        else if (commandName == "move_right") cmdType = CommandType::MOVE_RIGHT;
        else if (commandName == "cast_spell") cmdType = CommandType::CAST_SPELL;
        else if (commandName == "switch_mode") cmdType = CommandType::SWITCH_MODE;
        else if (commandName == "show_info") cmdType = CommandType::SHOW_INFO;
        else if (commandName == "show_help") cmdType = CommandType::SHOW_HELP;
        else if (commandName == "quit") cmdType = CommandType::QUIT;
        else if (commandName == "save_game") cmdType = CommandType::SAVE_GAME;
        else if (commandName == "load_game") cmdType = CommandType::LOAD_GAME;
        else continue;  // Неизвестная команда
        
        tempKeyToCommand[key] = cmdType;
        tempCommandToKey[cmdType] = key;
    }
    
    file.close();
    
    // Проверяем что все команды присутствуют
    if (tempCommandToKey.size() != 11) {  // 11 команд
        std::cerr << "[KeyBindingConfig] Incomplete configuration! Missing commands.\n";
        std::cerr << "[KeyBindingConfig] Using default keybindings.\n";
        loadDefaults();
        return false;
    }
    
    // Применяем загруженную конфигурацию
    keyToCommand = std::move(tempKeyToCommand);
    commandToKey = std::move(tempCommandToKey);
    
    // Валидируем
    if (!validate()) {
        std::cerr << "[KeyBindingConfig] Invalid configuration!\n";
        std::cerr << "[KeyBindingConfig] Using default keybindings.\n";
        loadDefaults();
        return false;
    }
    
    std::cout << "[KeyBindingConfig] Successfully loaded from: " << filepath << "\n";
    return true;
}

/**
 * @brief Валидировать конфигурацию
 */
bool KeyBindingConfig::validate() const {
    // Проверка 1: Все команды должны быть уникальны
    if (commandToKey.size() != keyToCommand.size()) {
        std::cerr << "[KeyBindingConfig] Validation failed: Duplicate keys or commands!\n";
        return false;
    }
    
    // Проверка 2: Должны быть все 11 команд
    if (commandToKey.size() != 11) {
        std::cerr << "[KeyBindingConfig] Validation failed: Missing commands!\n";
        return false;
    }
    
    return true;
}

/**
 * @brief Получить команду по клавише
 */
const CommandType* KeyBindingConfig::getCommand(const std::string& key) const {
    auto it = keyToCommand.find(key);
    if (it != keyToCommand.end()) {
        return &it->second;
    }
    return nullptr;
}

/**
 * @brief Получить клавишу по команде
 */
std::string KeyBindingConfig::getKey(CommandType command) const {
    auto it = commandToKey.find(command);
    if (it != commandToKey.end()) {
        return it->second;
    }
    return "";
}

/**
 * @brief Вывести все привязки
 */
void KeyBindingConfig::printBindings() const {
    std::cout << "\n=== KEY BINDINGS ===\n";
    
    auto printBinding = [this](CommandType cmd, const std::string& name) {
        std::cout << name << ": " << getKey(cmd) << "\n";
    };
    
    printBinding(CommandType::MOVE_UP, "Move Up");
    printBinding(CommandType::MOVE_DOWN, "Move Down");
    printBinding(CommandType::MOVE_LEFT, "Move Left");
    printBinding(CommandType::MOVE_RIGHT, "Move Right");
    printBinding(CommandType::CAST_SPELL, "Cast Spell");
    printBinding(CommandType::SWITCH_MODE, "Switch Mode");
    printBinding(CommandType::SHOW_INFO, "Show Info");
    printBinding(CommandType::SHOW_HELP, "Show Help");
    printBinding(CommandType::QUIT, "Quit");
    printBinding(CommandType::SAVE_GAME, "Save Game");
    printBinding(CommandType::LOAD_GAME, "Load Game");
    
    std::cout << "====================\n";
}


