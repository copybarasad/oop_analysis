#include "InputSystem.h"
#include <algorithm>

InputSystem::InputSystem() {
    initializeCommandMap();
}

void InputSystem::initializeCommandMap() {
    // Движение
    commandMap["w"] = Command::MOVE_UP;
    commandMap["a"] = Command::MOVE_LEFT;
    commandMap["s"] = Command::MOVE_DOWN;
    commandMap["d"] = Command::MOVE_RIGHT;
    
    // Боевые действия
    commandMap["f"] = Command::ATTACK;
    commandMap["m"] = Command::SWITCH_WEAPON;
    
    // Заклинания
    commandMap["1"] = Command::USE_SPELL_1;
    commandMap["2"] = Command::USE_SPELL_2;
    commandMap["3"] = Command::USE_SPELL_3;
    commandMap["i"] = Command::SHOW_SPELLS;
    commandMap["spells"] = Command::SHOW_SPELLS;
    
    // Магазин
    commandMap["b"] = Command::SHOP;
    commandMap["shop"] = Command::SHOP;
    
    // Системные команды
    commandMap["save"] = Command::SAVE_GAME;
    commandMap["load"] = Command::LOAD_GAME;
    commandMap["z"] = Command::SAVE_GAME;        
    commandMap["x"] = Command::LOAD_GAME;        
    commandMap["q"] = Command::EXIT_GAME;
    commandMap["exit"] = Command::EXIT_GAME;
    commandMap["quit"] = Command::EXIT_GAME;
}

Command InputSystem::processInput(const std::string& input) {
    std::string normalized = input;
    
    // Приводим к нижнему регистру и убираем пробелы
    std::transform(normalized.begin(), normalized.end(), normalized.begin(), ::tolower);
    normalized.erase(std::remove(normalized.begin(), normalized.end(), ' '), normalized.end());
    
    // Ищем команду в карте
    auto it = commandMap.find(normalized);
    if (it != commandMap.end()) {
        return it->second;
    }
    
    return Command::UNKNOWN;
}