#include "command_handler.h"
#include "spec_commands.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <vector>

std::map<std::string, std::string> CommandHandler::keyToCommand;

void CommandHandler::loadDefaultConfig() {
    keyToCommand = {
        {"W", "MOVE_UP"},
        {"S", "MOVE_DOWN"},
        {"A", "MOVE_LEFT"},
        {"D", "MOVE_RIGHT"},
        {"X", "ATTACK"},
        {"C", "CHANGE_TYPE"},
        {"STAT", "SHOW_STATUS"},
        {"MENU", "SHOW_MENU"},
        {"Q", "QUIT"},
        {"SPELLS", "SHOW_SPELLS"},
        {"CAST", "CAST_SPELL"},
        {"BUY", "BUY_SPELL"},
        {"SAVE", "SAVE"},
        {"LOAD", "LOAD"},
        {"RESTART", "RESTART"}
    };
}

bool CommandHandler::validateConfig() {
    std::map<std::string, int> keyCount;
    for (const auto& pair : keyToCommand) {
        keyCount[pair.first]++;
        if (keyCount[pair.first] > 1) {
            std::cout << "Ошибка: клавиша '" << pair.first << "' назначена на несколько команд" << std::endl;
            return false;
        }
    }
    
    std::vector<std::string> requiredCommands = {
        "MOVE_UP", "MOVE_DOWN", "MOVE_LEFT", "MOVE_RIGHT", 
        "ATTACK", "CHANGE_TYPE", "SHOW_STATUS", "SHOW_MENU",
        "QUIT", "SHOW_SPELLS", "CAST_SPELL", "BUY_SPELL",
        "SAVE", "LOAD", "RESTART"
    };
    
    for (const auto& cmd : requiredCommands) {
        bool found = false;
        for (const auto& [key, command] : keyToCommand) {
            if (command == cmd) {
                found = true;
                break;
            }
        }
        if (!found) return false;
    }
    
    return true;
}

void CommandHandler::loadConfig(const std::string& configFile) {
    std::ifstream file(configFile);
    if (!file.is_open()) {
        std::cout << "Файл конфигурации не найден. Используются настройки по умолчанию." << std::endl;
        loadDefaultConfig();
        return;
    }
    
    keyToCommand.clear();
    
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        
        std::istringstream iss(line);
        std::string command, key;
        if (iss >> command >> key) {
            keyToCommand[key] = command;
        }
    }
    
    file.close();
    
    if (!validateConfig()) {
        std::cout << "Конфигурация некорректна. Используются настройки по умолчанию." << std::endl;
        loadDefaultConfig();
    }
}

std::unique_ptr<Command> CommandHandler::parseCommand(const std::string& input) {
    if (keyToCommand.empty()) {
        loadDefaultConfig();
    }
    
    std::string upperInput = input;
    std::transform(upperInput.begin(), upperInput.end(), upperInput.begin(), ::toupper);
    
    auto it = keyToCommand.find(upperInput);
    if (it == keyToCommand.end()) {
        if (upperInput == "SHOW_STATUS") return std::make_unique<ShowStatusCommand>();
        if (upperInput == "SHOW_MENU") return std::make_unique<ShowMenuCommand>();
        if (upperInput == "SHOW_SPELLS") return std::make_unique<ShowSpellsCommand>();
        if (upperInput == "QUIT") return std::make_unique<QuitCommand>();
        if (upperInput == "SAVE") return std::make_unique<SaveGameCommand>();
        if (upperInput == "LOAD") return std::make_unique<LoadGameCommand>();
        if (upperInput == "RESTART") return std::make_unique<RestartCommand>();
        return nullptr;
    }
    
    std::string commandName = it->second;
    
    if (commandName == "MOVE_UP") {
        return std::make_unique<MoveCommand>(0, -1);
    } else if (commandName == "MOVE_DOWN") {
        return std::make_unique<MoveCommand>(0, 1);
    } else if (commandName == "MOVE_LEFT") {
        return std::make_unique<MoveCommand>(-1, 0);
    } else if (commandName == "MOVE_RIGHT") {
        return std::make_unique<MoveCommand>(1, 0);
    } else if (commandName == "ATTACK") {
        return std::make_unique<AttackCommand>();
    } else if (commandName == "CHANGE_TYPE") {
        return std::make_unique<ChangeCombatTypeCommand>();
    } else if (commandName == "SHOW_STATUS") {
        return std::make_unique<ShowStatusCommand>();
    } else if (commandName == "SHOW_SPELLS") {
        return std::make_unique<ShowSpellsCommand>();
    } else if (commandName == "CAST_SPELL") {
        int spellIndex, targetX, targetY;
        std::cout << "Введите индекс заклинания: ";
        std::cin >> spellIndex;
        std::cout << "Введите координаты цели (x y): ";
        std::cin >> targetX >> targetY;
        std::cin.ignore();
        return std::make_unique<CastSpellCommand>(spellIndex, targetX, targetY);
    } else if (commandName == "BUY_SPELL") {
        return std::make_unique<BuySpellCommand>();
    } else if (commandName == "SAVE") {
        return std::make_unique<SaveGameCommand>();
    } else if (commandName == "LOAD") {
        return std::make_unique<LoadGameCommand>();
    } else if (commandName == "QUIT") {
        return std::make_unique<QuitCommand>();
    } else if (commandName == "SHOW_MENU") {
        return std::make_unique<ShowMenuCommand>();
    } else if (commandName == "RESTART") {
        return std::make_unique<RestartCommand>();
    }
    
    return nullptr;
}

