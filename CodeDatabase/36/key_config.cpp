#include "key_config.h"
#include <fstream>
#include <sstream>
#include <set>
#include <iostream>
#include <algorithm>
#include <vector>
#include <iomanip>
// Подключаем filesystem для проверки пути (нужен C++17)
#include <filesystem> 

// Инициализация статического поля
std::map<char, GameAction> KeyConfig::key_map_;

KeyConfig::KeyConfig() {
    // Если настройки уже загружены, ничего не делаем
    if (!key_map_.empty()) {
        return;
    }

    try {
        LoadFromFile("keybindings.txt");
    } catch (const std::exception& e) {
        // ВАЖНО: Выводим реальную причину ошибки
        std::cerr << "[Config Error]: " << e.what() << "\n";
        std::cerr << "Loading defaults...\n";
        SetDefaults();
    }
}

void KeyConfig::LoadFromFile(const std::string& filename) {
    // Диагностика: где мы ищем файл?
    // std::cout << "Looking for config in: " << std::filesystem::current_path() << "\n";

    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("File '" + filename + "' not found in current directory.");
    }

    std::map<std::string, GameAction> string_to_enum = {
        {"MOVE_UP", GameAction::MoveUp},
        {"MOVE_DOWN", GameAction::MoveDown},
        {"MOVE_LEFT", GameAction::MoveLeft},
        {"MOVE_RIGHT", GameAction::MoveRight},
        {"ATTACK", GameAction::Attack},
        {"CAST", GameAction::CastSpell},
        {"SWITCH", GameAction::SwitchMode},
        {"SAVE", GameAction::SaveGame},
        {"LOAD", GameAction::LoadGame},
        {"QUIT", GameAction::Quit}
    };

    std::map<char, GameAction> temp_map;
    std::set<char> used_keys;
    std::set<GameAction> used_actions;

    std::string line;
    int line_num = 0;
    while (std::getline(file, line)) {
        line_num++;
        if (line.empty()) continue;
        
        std::stringstream ss(line);
        std::string action_str, key_str;
        
        if (std::getline(ss, action_str, '=') && std::getline(ss, key_str)) {
            // Очистка от пробелов и \r (для Windows)
            action_str.erase(0, action_str.find_first_not_of(" \t\r\n"));
            action_str.erase(action_str.find_last_not_of(" \t\r\n") + 1);
            key_str.erase(0, key_str.find_first_not_of(" \t\r\n"));
            key_str.erase(key_str.find_last_not_of(" \t\r\n") + 1);

            // Пропускаем комментарии или пустые строки
            if (action_str.empty() || action_str[0] == '#') continue;

            if (string_to_enum.find(action_str) == string_to_enum.end()) {
                std::cout << "Warning: Unknown action '" << action_str << "' on line " << line_num << "\n";
                continue; 
            }
            
            if (key_str.empty()) throw std::runtime_error("Empty key for " + action_str);

            GameAction act = string_to_enum[action_str];
            char key = tolower(key_str[0]); // Приводим к нижнему регистру для хранения

            // ВАЛИДАЦИЯ
            if (used_keys.count(key)) {
                throw std::runtime_error("Key '" + std::string(1, key) + "' is already used.");
            }
            if (used_actions.count(act)) {
                 throw std::runtime_error("Action '" + action_str + "' is already bound.");
            }

            temp_map[key] = act;
            used_keys.insert(key);
            used_actions.insert(act);
        }
    }

    // ВАЛИДАЦИЯ ПОЛНОТЫ (Проверка, что все команды назначены)
    for (int i = 0; i < static_cast<int>(GameAction::Count); ++i) {
        GameAction current = static_cast<GameAction>(i);
        
        // !!! ИСПРАВЛЕНИЕ: Не требуем назначать клавишу для None !!!
        if (current == GameAction::None) continue;

        if (used_actions.find(current) == used_actions.end()) {
            // Пытаемся найти имя пропущенной команды для вывода ошибки
            std::string missing_name = "Unknown";
            for(auto const& [name, val] : string_to_enum) {
                if (val == current) missing_name = name;
            }
            throw std::runtime_error("Missing binding for action: " + missing_name);
        }
    }

    key_map_ = temp_map;
    std::cout << "Custom controls loaded successfully from " << filename << ".\n";
}

void KeyConfig::SetDefaults() {
    key_map_.clear();
    key_map_['w'] = GameAction::MoveUp;
    key_map_['s'] = GameAction::MoveDown;
    key_map_['a'] = GameAction::MoveLeft;
    key_map_['d'] = GameAction::MoveRight;
    key_map_['e'] = GameAction::Attack;
    key_map_['c'] = GameAction::CastSpell;
    key_map_['x'] = GameAction::SwitchMode;
    key_map_['v'] = GameAction::SaveGame;
    key_map_['l'] = GameAction::LoadGame;
    key_map_['q'] = GameAction::Quit;
    std::cout << "Default controls loaded.\n";
}

GameAction KeyConfig::GetAction(char key) const {
    char lower_key = tolower(key);
    auto it = key_map_.find(lower_key);
    if (it != key_map_.end()) return it->second;
    return GameAction::None;
}

std::string KeyConfig::GetHelpString() const {
    std::stringstream ss;
    ss << "Controls:\n";

    std::vector<GameAction> actions = {
        GameAction::MoveUp, GameAction::MoveDown, GameAction::MoveLeft, GameAction::MoveRight,
        GameAction::Attack, GameAction::CastSpell, GameAction::SwitchMode,
        GameAction::SaveGame, GameAction::LoadGame, GameAction::Quit
    };

    auto actionName = [](GameAction act) -> std::string {
        switch (act) {
            case GameAction::MoveUp: return "Up";
            case GameAction::MoveDown: return "Down";
            case GameAction::MoveLeft: return "Left";
            case GameAction::MoveRight: return "Right";
            case GameAction::Attack: return "Attack";
            case GameAction::CastSpell: return "Cast Spell";
            case GameAction::SwitchMode: return "Switch Mode";
            case GameAction::SaveGame: return "Save";
            case GameAction::LoadGame: return "Load";
            case GameAction::Quit: return "Quit";
            default: return "Unknown";
        }
    };

    int count = 0;
    for (const auto& act : actions) {
        char key = '?';
        for (const auto& [k, v] : key_map_) {
            if (v == act) {
                key = toupper(k);
                break;
            }
        }
        ss << "[" << key << "] " << std::left << std::setw(15) << actionName(act);
        count++;
        if (count % 4 == 0) ss << "\n";
    }
    
    return ss.str();
}