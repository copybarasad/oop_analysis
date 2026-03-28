#include "KeyConfig.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <set>
#include <algorithm>

namespace Game {

KeyConfig::KeyConfig() {
    // При создании объекта сразу устанавливаем настройки по умолчанию.
    // Если файл не загрузится, они останутся.
    setDefault();
}

void KeyConfig::setDefault() {
    keyToCommand.clear();
    keyToCommand['w'] = CommandType::MoveUp;
    keyToCommand['s'] = CommandType::MoveDown;
    keyToCommand['a'] = CommandType::MoveLeft;
    keyToCommand['d'] = CommandType::MoveRight;
    keyToCommand['f'] = CommandType::SwitchCombat;
    keyToCommand['c'] = CommandType::CastSpell;
    keyToCommand['k'] = CommandType::SaveQuit;
    keyToCommand['x'] = CommandType::Quit;
}

void KeyConfig::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cout << "Info: Could not open '" << filename << "'. Using default controls." << std::endl;
        // setDefault() уже был вызван в конструкторе, так что ничего делать не нужно.
        return;
    }

    // Временные карты для безопасного чтения и последующей валидации
    std::map<char, CommandType> tempKeyToCommand;
    std::map<CommandType, char> tempCommandToKey;

    std::map<std::string, CommandType> stringToCommand = {
        {"move_up", CommandType::MoveUp}, {"move_down", CommandType::MoveDown},
        {"move_left", CommandType::MoveLeft}, {"move_right", CommandType::MoveRight},
        {"switch_combat", CommandType::SwitchCombat}, {"cast_spell", CommandType::CastSpell},
        {"save_quit", CommandType::SaveQuit}, {"quit", CommandType::Quit}
    };

    std::string line;
    int lineNumber = 0;
    while (std::getline(file, line)) {
        lineNumber++;
        std::stringstream ss(line);
        std::string commandStr, eq, keyStr;
        
        // Пропускаем пустые строки и комментарии
        if (line.empty() || line[0] == '#') continue;

        if (ss >> commandStr >> eq >> keyStr && eq == "=" && keyStr.length() == 1) {
            char key = std::tolower(keyStr[0]);
            
            if (stringToCommand.count(commandStr)) {
                CommandType cmd = stringToCommand.at(commandStr);
                tempKeyToCommand[key] = cmd;
                tempCommandToKey[cmd] = key;
            } else {
                 std::cerr << "Warning: Unknown command '" << commandStr << "' in config file at line " << lineNumber << "." << std::endl;
            }
        } else {
            std::cerr << "Warning: Malformed line in config file at line " << lineNumber << ": " << line << std::endl;
        }
    }

    // Проводим строгую валидацию.
    // 1. Должны быть все 8 команд.
    // 2. Должно быть 8 уникальных клавиш (размер key->cmd карты).
    // 3. Должно быть 8 уникальных команд (размер cmd->key карты).
    if (tempKeyToCommand.size() == 8 && tempCommandToKey.size() == 8) {
        // Только если все проверки пройдены, заменяем настройки по умолчанию на загруженные
        keyToCommand = tempKeyToCommand;
        std::cout << "Key config loaded successfully from '" << filename << "'." << std::endl;
    } else {
        std::cerr << "Error: Invalid or incomplete key config in '" << filename << "'. "
                  << "Ensure all 8 commands are bound to unique keys. Reverting to default controls." << std::endl;
        // Ничего не делаем, настройки по умолчанию, установленные в конструкторе, остаются в силе.
    }
}

CommandType KeyConfig::getCommandType(char key) const {
    auto it = keyToCommand.find(key);
    if (it != keyToCommand.end()) {
        return it->second;
    }
    // Это исключение никогда не должно вызываться, если isKeyBound используется правильно
    throw std::out_of_range("Key not bound");
}

bool KeyConfig::isKeyBound(char key) const {
    return keyToCommand.count(key);
}

}