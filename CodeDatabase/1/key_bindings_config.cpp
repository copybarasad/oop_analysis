#include "key_bindings_config.h"
#include "event_system.h"
#include "game_events.h"
#include <algorithm>
#include <set>
#include <cctype>
#include <iostream>
#include <fstream>

std::string KeyBindingsConfig::normalizeKey(const std::string& key) const {
    if (key.empty()) return key;
    
    if (key == " " || key == "SPACE") {
        return " ";
    }
    
    std::string result = key;
    for (char& c : result) {
        c = std::toupper(static_cast<unsigned char>(c));
    }
    return result;
}

KeyBindingsConfig::KeyBindingsConfig() {
    initializeDefaultBindings();
}

void KeyBindingsConfig::initializeDefaultBindings() {
    default_bindings_ = {
        {"W", "MOVE_UP"},
        {"S", "MOVE_DOWN"},
        {"A", "MOVE_LEFT"},
        {"D", "MOVE_RIGHT"},
        {" ", "ATTACK"},
        {"B", "SWITCH_COMBAT"},
        {"F", "CAST_SPELL"},
        {"I", "SHOW_STATUS"},
        {"M", "LOOK_AROUND"},
        {"P", "SHOW_SPELLS"},
        {"H", "HELP"},
        {"O", "SAVE_GAME"},
        {"L", "LOAD_GAME"},
        {"Q", "QUIT"}
    };
}

bool KeyBindingsConfig::createDefaultConfigFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) return false;
    
    file << "W MOVE_UP\n";
    file << "S MOVE_DOWN\n";
    file << "A MOVE_LEFT\n";
    file << "D MOVE_RIGHT\n";
    file << "SPACE ATTACK\n";     
    file << "B SWITCH_COMBAT\n";
    file << "F CAST_SPELL\n";
    file << "I SHOW_STATUS\n";
    file << "M LOOK_AROUND\n";
    file << "P SHOW_SPELLS\n";
    file << "H HELP\n";
    file << "O SAVE_GAME\n";
    file << "L LOAD_GAME\n";
    file << "Q QUIT\n";
    
    file.close();
    return true;
}

void KeyBindingsConfig::loadFromFile(const std::string& filename, EventSystem* event_system) {
    auto trim = [](const std::string& str) -> std::string {
        size_t first = str.find_first_not_of(" \t");
        if (first == std::string::npos) return "";
        size_t last = str.find_last_not_of(" \t");
        return str.substr(first, last - first + 1);
    };
    
    key_bindings_.clear();
    
    std::ifstream testFile(filename);
    bool fileExists = testFile.is_open();
    testFile.close();
    
    if (!fileExists) {
        if (createDefaultConfigFile(filename)) {
            if (event_system) {
                GeneralEvent event("CONFIG", "Создан файл конфигурации: " + filename);
                event_system->notify(event);
            }
            std::cout << "📄 Создан файл конфигурации\n";
        } else {
            if (event_system) {
                GeneralEvent event("CONFIG", "Не удалось создать файл: " + filename);
                event_system->notify(event);
            }
            setDefaultBindings();
            return;
        }
    }
    
    std::ifstream file(filename);
    if (!file.is_open()) {
        if (event_system) {
            GeneralEvent event("CONFIG", "Не удалось открыть файл: " + filename);
            event_system->notify(event);
        }
        setDefaultBindings();
        return;
    }
    
    std::map<std::string, std::string> loaded_bindings;
    std::string line;
    int line_number = 0;
    int loaded_count = 0;
    int error_count = 0;
    
    while (std::getline(file, line)) {
        line_number++;
        
        // 1. Удаляем комментарии (всё после #)
        size_t comment_pos = line.find('#');
        if (comment_pos != std::string::npos) {
            line = line.substr(0, comment_pos);
        }
        
        // 2. Убираем пробелы по краям с помощью лямбды
        line = trim(line);
        if (line.empty()) {
            continue;
        }
        
        // 3. Парсим ключ и команду
        std::string key, command;
        
        // Ищем первый пробел или табуляцию как разделитель
        size_t delimiter_pos = line.find(' ');
        if (delimiter_pos == std::string::npos) {
            delimiter_pos = line.find('\t');
        }
        
        if (delimiter_pos == std::string::npos) {
            // Нет разделителя - ошибка формата
            error_count++;
            continue;
        }
        
        // Извлекаем ключ (до разделителя)
        key = line.substr(0, delimiter_pos);
        
        // Извлекаем команду (после разделителя)
        command = line.substr(delimiter_pos + 1);
        
        // Убираем пробелы в команде с помощью лямбды
        command = trim(command);
        
        // 4. Проверяем, что ключ и команда не пустые
        if (key.empty() || command.empty()) {
            error_count++;
            continue;
        }
        
        // 5. Нормализуем ключ
        std::string normalized_key = normalizeKey(key);
        
        // 6. Сохраняем
        loaded_bindings[normalized_key] = command;
        loaded_count++;
    }
    
    file.close();
    
    if (event_system) {
        std::string message = "Загружено " + std::to_string(loaded_count) + 
                             " привязок из файла";
        if (error_count > 0) {
            message += ", " + std::to_string(error_count) + " ошибок";
        }
        GeneralEvent event("CONFIG", message);
        event_system->notify(event);
    }
    
    key_bindings_ = loaded_bindings;
    
    if (!validateConfig()) {
        if (event_system) {
            auto errors = getValidationErrors();
            for (const auto& error : errors) {
                GeneralEvent event("CONFIG", "Ошибка валидации: " + error);
                event_system->notify(event);
            }
            GeneralEvent event("CONFIG", "Используются настройки по умолчанию");
            event_system->notify(event);
        }
        
        setDefaultBindings();
    } else {
        if (event_system) {
            GeneralEvent event("CONFIG", "Конфигурация успешно загружена и проверена");
            event_system->notify(event);
        }
    }
}

void KeyBindingsConfig::saveToFile(const std::string& filename, EventSystem* event_system) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        if (event_system) {
            GeneralEvent event("CONFIG", "Не удалось сохранить конфигурацию");
            event_system->notify(event);
        }
        return;
    }
    
    for (const auto& binding : key_bindings_) {
        if (binding.first == " ") {
            file << "SPACE " << binding.second << "\n"; 
        } else {
            file << binding.first << " " << binding.second << "\n";
        }
    }
    
    file.close();
    
    if (event_system) {
        GeneralEvent event("CONFIG", "Конфигурация сохранена");
        event_system->notify(event);
    }
}

std::string KeyBindingsConfig::getCommandForKey(const std::string& key) const {
    std::string normalized_key = normalizeKey(key);
    auto it = key_bindings_.find(normalized_key);
    if (it != key_bindings_.end()) {
        return it->second;
    }
    return "";
}

bool KeyBindingsConfig::validateConfig() const {
    return !hasDuplicateKeys() && 
           !hasDuplicateCommands() && 
           !hasMissingCommands() &&
           !hasUnknownCommands();
}

bool KeyBindingsConfig::hasDuplicateKeys() const {
    std::set<std::string> keys;
    for (const auto& binding : key_bindings_) {
        if (!keys.insert(binding.first).second) {
            return true;
        }
    }
    return false;
}

bool KeyBindingsConfig::hasDuplicateCommands() const {
    std::set<std::string> commands;
    for (const auto& binding : key_bindings_) {
        if (!commands.insert(binding.second).second) {
            return true;
        }
    }
    return false;
}

bool KeyBindingsConfig::hasMissingCommands() const {
    std::set<std::string> required_commands = {
        "MOVE_UP", "MOVE_DOWN", "MOVE_LEFT", "MOVE_RIGHT", "ATTACK"
    };
    
    std::set<std::string> current_commands;
    for (const auto& binding : key_bindings_) {
        current_commands.insert(binding.second);
    }
    
    for (const auto& required : required_commands) {
        if (current_commands.find(required) == current_commands.end()) {
            return true;
        }
    }
    return false;
}

bool KeyBindingsConfig::hasUnknownCommands() const {
    std::set<std::string> valid_commands = {
        "MOVE_UP", "MOVE_DOWN", "MOVE_LEFT", "MOVE_RIGHT",
        "ATTACK", "SWITCH_COMBAT", "CAST_SPELL", "SHOW_STATUS",
        "LOOK_AROUND", "SHOW_SPELLS", "HELP", "SAVE_GAME", 
        "LOAD_GAME", "QUIT"
    };
    
    for (const auto& binding : key_bindings_) {
        if (valid_commands.find(binding.second) == valid_commands.end()) {
            return true;
        }
    }
    return false;
}

std::vector<std::string> KeyBindingsConfig::getValidationErrors() const {
    std::vector<std::string> errors;
    
    if (hasDuplicateKeys()) errors.push_back("Дублирующиеся клавиши");
    if (hasDuplicateCommands()) errors.push_back("Дублирующиеся команды");
    if (hasMissingCommands()) errors.push_back("Отсутствуют обязательные команды");
    if (hasUnknownCommands()) errors.push_back("Неизвестные команды");
    
    return errors;
}

void KeyBindingsConfig::setBinding(const std::string& key, const std::string& command) {
    std::string normalized_key = normalizeKey(key);
    key_bindings_[normalized_key] = command;
}

void KeyBindingsConfig::setDefaultBindings() {
    key_bindings_ = default_bindings_;
}

void KeyBindingsConfig::clearBindings() {
    key_bindings_.clear();
}

std::vector<std::pair<std::string, std::string>> 
KeyBindingsConfig::getBindingsForDisplay() const {
    std::vector<std::pair<std::string, std::string>> result;
    for (const auto& binding : key_bindings_) {
        std::string key_display = (binding.first == " ") ? "[ПРОБЕЛ]" : binding.first;
        result.push_back({key_display, binding.second});
    }
    return result;
}

std::map<std::string, std::vector<std::pair<std::string, std::string>>> 
KeyBindingsConfig::getBindingsByCategory() const {
    std::map<std::string, std::vector<std::pair<std::string, std::string>>> categories;
    
    for (const auto& binding : key_bindings_) {
        std::string key_display = (binding.first == " ") ? "[ПРОБЕЛ]" : binding.first;
        std::string command = binding.second;
        
        std::string category;
        if (command.find("MOVE_") == 0) category = "ДВИЖЕНИЕ";
        else if (command == "ATTACK" || command == "SWITCH_COMBAT" || command == "CAST_SPELL") category = "БОЙ";
        else if (command == "SHOW_STATUS" || command == "LOOK_AROUND" || command == "SHOW_SPELLS" || command == "HELP") category = "ИНФОРМАЦИЯ";
        else if (command == "SAVE_GAME" || command == "LOAD_GAME" || command == "QUIT") category = "СИСТЕМА";
        else category = "ДРУГОЕ";
        
        categories[category].push_back({key_display, command});
    }
    
    return categories;
}