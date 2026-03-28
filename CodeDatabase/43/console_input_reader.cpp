#include "console_input_reader.h"
#include <iostream>
#include <fstream>
#include <cctype>

ConsoleInputReader::ConsoleInputReader() {
    setDefaultBindings();
}

char ConsoleInputReader::readChar() const {
    char ch;
    std::cin >> ch;
    return std::tolower(ch);
}

Command ConsoleInputReader::readCommand() {
    char input = readChar();
    
    auto it = keyBindings.find(input);
    if (it != keyBindings.end()) {
        return Command(it->second);
    }
    
    return Command(Command::Type::INVALID);
}

void ConsoleInputReader::setDefaultBindings() {
    keyBindings.clear();
    keyBindings['w'] = Command::Type::MOVE_UP;
    keyBindings['s'] = Command::Type::MOVE_DOWN;
    keyBindings['a'] = Command::Type::MOVE_LEFT;
    keyBindings['d'] = Command::Type::MOVE_RIGHT;
    keyBindings['m'] = Command::Type::SWITCH_MODE;
    keyBindings['f'] = Command::Type::RANGED_ATTACK;
    keyBindings['1'] = Command::Type::CAST_SPELL_1;
    keyBindings['2'] = Command::Type::CAST_SPELL_2;
    keyBindings['3'] = Command::Type::CAST_SPELL_3;
    keyBindings['4'] = Command::Type::CAST_SPELL_4;
    keyBindings['5'] = Command::Type::CAST_SPELL_5;
    keyBindings['z'] = Command::Type::SAVE_GAME;
    keyBindings['x'] = Command::Type::LOAD_GAME;
    keyBindings['r'] = Command::Type::RESTART_GAME;
    keyBindings['q'] = Command::Type::QUIT_GAME;
}

void ConsoleInputReader::loadKeyBindings(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        return; // Используем дефолтные настройки
    }
    
    // Простая загрузка из файла
    std::map<char, Command::Type> newBindings;
    std::string line;
    
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        
        size_t eqPos = line.find('=');
        if (eqPos == std::string::npos) continue;
        
        std::string keyStr = line.substr(0, eqPos);
        std::string cmdStr = line.substr(eqPos + 1);
        
        if (keyStr.empty()) continue;
        
        char key = std::tolower(keyStr[0]);
        
        // Определяем тип команды
        Command::Type cmdType = Command::Type::INVALID;
        if (cmdStr == "move_up") cmdType = Command::Type::MOVE_UP;
        else if (cmdStr == "move_down") cmdType = Command::Type::MOVE_DOWN;
        else if (cmdStr == "move_left") cmdType = Command::Type::MOVE_LEFT;
        else if (cmdStr == "move_right") cmdType = Command::Type::MOVE_RIGHT;
        else if (cmdStr == "switch_mode") cmdType = Command::Type::SWITCH_MODE;
        else if (cmdStr == "ranged_attack") cmdType = Command::Type::RANGED_ATTACK;
        else if (cmdStr == "cast_1") cmdType = Command::Type::CAST_SPELL_1;
        else if (cmdStr == "cast_2") cmdType = Command::Type::CAST_SPELL_2;
        else if (cmdStr == "cast_3") cmdType = Command::Type::CAST_SPELL_3;
        else if (cmdStr == "cast_4") cmdType = Command::Type::CAST_SPELL_4;
        else if (cmdStr == "cast_5") cmdType = Command::Type::CAST_SPELL_5;
        else if (cmdStr == "save") cmdType = Command::Type::SAVE_GAME;
        else if (cmdStr == "load") cmdType = Command::Type::LOAD_GAME;
        else if (cmdStr == "restart") cmdType = Command::Type::RESTART_GAME;
        else if (cmdStr == "quit") cmdType = Command::Type::QUIT_GAME;
        
        if (cmdType != Command::Type::INVALID) {
            newBindings[key] = cmdType;
        }
    }
    
    // Если загружены корректные привязки, применяем их
    if (!newBindings.empty()) {
        keyBindings = std::move(newBindings);
    }
}