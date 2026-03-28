#include "CommandReader.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <typeinfo>

ConsoleCommandReader::ConsoleCommandReader() {
    setDefaultBindings();
    initializeDescriptions();
}

ConsoleCommandReader::ConsoleCommandReader(const std::string& configFile) {
    loadKeyBindings(configFile);
    initializeDescriptions();
}

// Реализация метода интерфейса
void ConsoleCommandReader::loadConfiguration(const std::string& filename) {
    loadKeyBindings(filename);
}

void ConsoleCommandReader::setDefaultBindings() {
    keyBindings = {
        {"w", CommandType::MOVE_UP},
        {"s", CommandType::MOVE_DOWN},
        {"a", CommandType::MOVE_LEFT},
        {"d", CommandType::MOVE_RIGHT},
        {"m", CommandType::SWITCH_MODE},
        {"c", CommandType::CAST_SPELL},
        {"r", CommandType::RANGED_ATTACK},
        {"p", CommandType::SHOW_SPELLS},
        {"save", CommandType::SAVE_GAME},
        {"load", CommandType::LOAD_GAME},
        {"x", CommandType::EXIT}
    };
}

void ConsoleCommandReader::initializeDescriptions() {
    commandDescriptions = {
        {CommandType::MOVE_UP, "Move up"},
        {CommandType::MOVE_DOWN, "Move down"},
        {CommandType::MOVE_LEFT, "Move left"},
        {CommandType::MOVE_RIGHT, "Move right"},
        {CommandType::SWITCH_MODE, "Switch fight mode"},
        {CommandType::CAST_SPELL, "Cast a spell"},
        {CommandType::RANGED_ATTACK, "Ranged attack"},
        {CommandType::SHOW_SPELLS, "Show spells"},
        {CommandType::SAVE_GAME, "Save game"},
        {CommandType::LOAD_GAME, "Load game"},
        {CommandType::EXIT, "Exit game"}
    };
}

void ConsoleCommandReader::loadKeyBindings(const std::string& configFile) {
    std::ifstream file(configFile);
    if (!file.is_open()) {
        std::cout << "Cannot open config file: " << configFile
                  << ". Using default bindings.\n";
        setDefaultBindings();
        return;
    }
    
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        
        std::istringstream iss(line);
        std::string commandStr, key;
        
        if (std::getline(iss, commandStr, '=') && std::getline(iss, key)) {
            commandStr.erase(std::remove_if(commandStr.begin(), commandStr.end(), ::isspace), commandStr.end());
            key.erase(std::remove_if(key.begin(), key.end(), ::isspace), key.end());
            
            CommandType cmd;
            if (commandStr == "MOVE_UP") cmd = CommandType::MOVE_UP;
            else if (commandStr == "MOVE_DOWN") cmd = CommandType::MOVE_DOWN;
            else if (commandStr == "MOVE_LEFT") cmd = CommandType::MOVE_LEFT;
            else if (commandStr == "MOVE_RIGHT") cmd = CommandType::MOVE_RIGHT;
            else if (commandStr == "SWITCH_MODE") cmd = CommandType::SWITCH_MODE;
            else if (commandStr == "CAST_SPELL") cmd = CommandType::CAST_SPELL;
            else if (commandStr == "RANGED_ATTACK") cmd = CommandType::RANGED_ATTACK;
            else if (commandStr == "SHOW_SPELLS") cmd = CommandType::SHOW_SPELLS;
            else if (commandStr == "SAVE_GAME") cmd = CommandType::SAVE_GAME;
            else if (commandStr == "LOAD_GAME") cmd = CommandType::LOAD_GAME;
            else if (commandStr == "EXIT") cmd = CommandType::EXIT;
            else {
                std::cout << "Unknown command in config: " << commandStr << std::endl;
                continue;
            }
            
            keyBindings[key] = cmd;
        }
    }
    
    file.close();
    
    // Проверяем все ли команды имеют привязки
    setDefaultBindings();
}

GameCommand ConsoleCommandReader::readCommand() {
    std::string input;
    std::cout << ">>> Enter command: ";
    std::cin >> input;
    
    if (input == "help") {
        displayHelp();
        return GameCommand(CommandType::SHOW_SPELLS);
    }
    
    auto it = keyBindings.find(input);
    if (it != keyBindings.end()) {
        CommandType cmdType = it->second;
        
        if (cmdType == CommandType::CAST_SPELL) {
            std::cout << "Enter spell number: ";
            size_t spellIndex;
            std::cin >> spellIndex;
            return GameCommand(cmdType, spellIndex - 1);
        }
        else if (cmdType == CommandType::RANGED_ATTACK) {
            std::cout << "Enter target coordinates (x y): ";
            int x, y;
            std::cin >> x >> y;
            return GameCommand(cmdType, Position(x, y));
        }
        
        return GameCommand(cmdType);
    }
    
    std::cout << "Unknown command. Type 'help' for commands.\n";
    return GameCommand(CommandType::SHOW_SPELLS);
}

void ConsoleCommandReader::displayHelp() {
    std::cout << "\n=== AVAILABLE COMMANDS ===\n";
    
    std::map<std::string, std::vector<std::string>> keyToCommands;
    for (const auto& binding : keyBindings) {
        std::string cmdName;
        switch (binding.second) {
            case CommandType::MOVE_UP: cmdName = "MOVE_UP"; break;
            case CommandType::MOVE_DOWN: cmdName = "MOVE_DOWN"; break;
            case CommandType::MOVE_LEFT: cmdName = "MOVE_LEFT"; break;
            case CommandType::MOVE_RIGHT: cmdName = "MOVE_RIGHT"; break;
            case CommandType::SWITCH_MODE: cmdName = "SWITCH_MODE"; break;
            case CommandType::CAST_SPELL: cmdName = "CAST_SPELL"; break;
            case CommandType::RANGED_ATTACK: cmdName = "RANGED_ATTACK"; break;
            case CommandType::SHOW_SPELLS: cmdName = "SHOW_SPELLS"; break;
            case CommandType::SAVE_GAME: cmdName = "SAVE_GAME"; break;
            case CommandType::LOAD_GAME: cmdName = "LOAD_GAME"; break;
            case CommandType::EXIT: cmdName = "EXIT"; break;
        }
        keyToCommands[binding.first].push_back(cmdName);
    }
    
    for (const auto& pair : keyToCommands) {
        std::cout << pair.first << " - ";
        for (size_t i = 0; i < pair.second.size(); ++i) {
            std::cout << pair.second[i];
            if (i < pair.second.size() - 1) std::cout << ", ";
        }
        std::cout << std::endl;
    }
    
    std::cout << "help - Show this help\n";
    std::cout << "========================\n\n";
}
