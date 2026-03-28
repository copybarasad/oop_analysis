#include "InputHandler.h"
#include "GameVisualize.h"
#include "Command.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <limits>

namespace {
    const std::unordered_map<char, CommandType> defaultKeyBindings = {
        {'w', CommandType::MOVE_UP},
        {'s', CommandType::MOVE_DOWN},
        {'a', CommandType::MOVE_LEFT},
        {'d', CommandType::MOVE_RIGHT},
        {'t', CommandType::CAST_SPELL},
        {'c', CommandType::SWITCH_COMBAT},
        {'b', CommandType::BUY_SPELL},
        {'v', CommandType::SAVE_GAME},
        {'m', CommandType::SHOW_MENU},
        {'q', CommandType::QUIT_GAME},
        {'0', CommandType::CANCEL}
    };
}

InputHandler::InputHandler(const std::string& configFile) 
    : configFilePath(configFile) {
    initializeKeyBindings();
}

void InputHandler::clearInputBuffer() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

std::string InputHandler::getLineInternal() {
    std::string input;
    std::getline(std::cin, input);
    return input;
}

InputHandler::ConfigLoadResult InputHandler::loadConfigFromFile() {
    ConfigLoadResult result;
    std::ifstream file(configFilePath);
    
    if (!file.is_open()) {
        result.success = false;
        result.errors.push_back("Config file not found: " + configFilePath);
        return result;
    }
    
    std::unordered_map<char, CommandType> loadedBindings;
    std::unordered_map<CommandType, char> commandToKey;
    std::unordered_map<char, CommandType> keyToCommand;
    
    std::string line;
    int lineNumber = 0;
    
    while (std::getline(file, line)) {
        lineNumber++;
        
        if (line.empty() || line[0] == '#') {
            continue;
        }
        
        std::istringstream iss(line);
        std::string keyStr, commandStr;
        
        if (iss >> keyStr >> commandStr) {
            if (keyStr.length() != 1) {
                result.errors.push_back("Line " + std::to_string(lineNumber) + 
                                      ": Key must be single character");
                continue;
            }
            
            char key = keyStr[0];
            CommandType cmdType = CommandUtils::stringToCommandType(commandStr);
            
            if (cmdType == CommandType::UNKNOWN) {
                result.errors.push_back("Line " + std::to_string(lineNumber) + 
                                      ": Unknown command '" + commandStr + "'");
                continue;
            }
            
            // Проверка: одна клавиша - несколько команд?
            auto keyIt = keyToCommand.find(key);
            if (keyIt != keyToCommand.end()) {
                result.errors.push_back("Line " + std::to_string(lineNumber) + 
                                      ": Key '" + keyStr + "' already assigned to '" + 
                                      CommandUtils::commandTypeToString(keyIt->second) + "'");
                continue;
            }
            
            // Проверка: одна команда - несколько клавиш?
            auto cmdIt = commandToKey.find(cmdType);
            if (cmdIt != commandToKey.end()) {
                result.warnings.push_back("Line " + std::to_string(lineNumber) + 
                                        ": Command '" + commandStr + 
                                        "' already assigned to key '" + 
                                        std::string(1, cmdIt->second) + 
                                        "'. Overwriting.");
            }
        
            loadedBindings[key] = cmdType;
            commandToKey[cmdType] = key;
            keyToCommand[key] = cmdType;
            
            // добавляем версию в верхнем регистре
            if (std::isalpha(key)) {
                char upperKey = std::toupper(key);
                loadedBindings[upperKey] = cmdType;
                keyToCommand[upperKey] = cmdType;
            }
        } else {
            result.errors.push_back("Line " + std::to_string(lineNumber) + 
                                   ": Invalid format. Expected: KEY COMMAND");
        }
    }
    
    file.close();
    
    // Проверка: все ли обязательные команды есть?
    std::vector<CommandType> allCommands = CommandUtils::getAllCommandTypes();
    for (const auto& cmd : allCommands) {
        if (commandToKey.find(cmd) == commandToKey.end()) {
            result.missingCommands.push_back(cmd);
        }
    }
    
    if (!result.errors.empty() || !result.missingCommands.empty()) {
        result.success = false;
    } else {
        result.success = true;
        result.bindings = loadedBindings;
    }
    
    return result;
}

void InputHandler::saveBindingsToFile(const std::unordered_map<char, CommandType>& bindings) {
    std::ofstream file(configFilePath);
    if (!file.is_open()) {
        return;
    }
    
    file << "# Game Controls Configuration\n";
    file << "# Format: KEY COMMAND_NAME\n";
    file << "# Available commands:\n";
    
    for (const auto& cmd : CommandUtils::getAllCommandTypes()) {
        file << "#   " << CommandUtils::commandTypeToString(cmd) << "\n";
    }
    file << "\n";
    
    // Собираем уникальные привязки
    std::map<CommandType, char> uniqueBindings;
    for (const auto& binding : bindings) {
        char key = binding.first;
        CommandType cmd = binding.second;
        
        if (std::isalpha(key) && std::islower(key)) {
            uniqueBindings[cmd] = key;
        } else if (!std::isalpha(key)) {
            uniqueBindings[cmd] = key;
        }
    }
    
    for (const auto& binding : uniqueBindings) {
        CommandType cmd = binding.first;
        char key = binding.second;
        file << key << " " << CommandUtils::commandTypeToString(cmd) << "\n";
    }
    
    file.close();
}

bool InputHandler::initializeKeyBindings() {
    auto result = loadConfigFromFile();
    
    if (!result.success) {
        ConsoleGameVisualizer visualizer;
        
        for (const auto& error : result.errors) {
            visualizer.visualizeError("Config File Error", error);
        }
        
        visualizer.visualizeOperationResult(
            "Configuration", 
            "reset", 
            "Using default controls (WASD)"
        );
        
        saveBindingsToFile(defaultKeyBindings);
        keyBindings = defaultKeyBindings;
        return false;
    }
    
    keyBindings = result.bindings;
    return true;
}

char InputHandler::findKeyForCommand(CommandType command) const {
    for (const auto& binding : keyBindings) {
        char key = binding.first;
        CommandType cmd = binding.second;
        
        if (cmd == command && std::islower(key)) {
            return key;
        }
    }
    return '\0';
}

std::string InputHandler::getCommandDescription(CommandType cmd) const {
    static const std::map<CommandType, std::string> descriptions = {
        {CommandType::MOVE_UP, "Move character up"},
        {CommandType::MOVE_DOWN, "Move character down"},
        {CommandType::MOVE_LEFT, "Move character left"},
        {CommandType::MOVE_RIGHT, "Move character right"},
        {CommandType::CAST_SPELL, "Cast a spell"},
        {CommandType::SWITCH_COMBAT, "Switch combat type (melee/ranged)"},
        {CommandType::BUY_SPELL, "Buy a new spell"},
        {CommandType::SAVE_GAME, "Save the game"},
        {CommandType::SHOW_MENU, "Show extended menu"},
        {CommandType::QUIT_GAME, "Quit the game"}
    };
    
    auto it = descriptions.find(cmd);
    return it != descriptions.end() ? it->second : "Unknown command";
}

Command InputHandler::getGameCommand() {
    std::string input = getLineInternal();
    
    if (input.length() == 1) {
        char key = input[0];
        auto it = keyBindings.find(key);
        if (it != keyBindings.end()) {
            return Command(it->second);
        }
    }
    
    return Command(CommandType::INVALID);
}

Command InputHandler::getMenuCommand() {
    std::string input = getLineInternal();
    
    try {
        int value = std::stoi(input);
        return Command(CommandType::SELECT_OPTION, value);
    } catch (const std::exception&) {
        return Command(CommandType::INVALID);
    }
}

Command InputHandler::getCoordinatesCommand() {
    std::string input = getLineInternal();
    std::istringstream iss(input);
    int x, y;
    
    if (iss >> x >> y) {
        std::string extra;
        if (!(iss >> extra)) {
            return Command(CommandType::ENTER_COORDINATES, x, y, true);
        }
    }
    
    return Command(CommandType::INVALID);
}

Command InputHandler::getSizeCommand() {
    std::string input = getLineInternal();
    std::istringstream iss(input);
    int width, height;
    
    if (iss >> width >> height) {
        std::string extra;
        if (!(iss >> extra)) {
            return Command(CommandType::ENTER_SIZE, width, height, false);
        }
    }
    
    return Command(CommandType::INVALID);
}

Command InputHandler::getSpellCommand(int maxSpells) {
    std::string spellInput = getLineInternal();
    int spellIndex = -1;
    
    try {
        spellIndex = std::stoi(spellInput) - 1;
        if (spellIndex < 0 || spellIndex >= maxSpells) {
            return Command(CommandType::INVALID);
        }
    } catch (const std::exception&) {
        return Command(CommandType::INVALID);
    }
    
    // нужны ли координаты
    std::string answer = getLineInternal();
    
    if (answer == "y" || answer == "Y") {
        std::string coordInput = getLineInternal();
        std::istringstream iss(coordInput);
        int x, y;
        
        if (iss >> x >> y) {
            std::string extra;
            if (!(iss >> extra)) {
                return Command(CommandType::CAST_SPELL, spellIndex, true, x, y);
            }
        }
        return Command(CommandType::INVALID);
    } else {
        return Command(CommandType::CAST_SPELL, spellIndex, false, 0, 0);
    }
}

bool InputHandler::getConfirmation() {
    std::string input = getLineInternal();
    return (input == "y" || input == "Y");
}

InputHandler::ConfigStatus InputHandler::getConfigStatus() {
    auto result = loadConfigFromFile();
    ConfigStatus status;
    
    status.isValid = result.success;
    status.errors = result.errors;
    status.warnings = result.warnings;
    
    for (const auto& cmd : result.missingCommands) {
        status.missingCommands.emplace_back(
            CommandUtils::commandTypeToString(cmd),
            getCommandDescription(cmd)
        );
    }
    
    return status;
}

std::vector<std::pair<std::string, std::vector<std::string>>> InputHandler::getCurrentBindings() const {
    std::map<CommandType, std::vector<std::string>> commandToKeys;
    
    for (const auto& binding : keyBindings) {
        char key = binding.first;
        CommandType cmd = binding.second;
        std::string keyStr(1, key);
        commandToKeys[cmd].push_back(keyStr);
    }
    
    std::vector<std::pair<std::string, std::vector<std::string>>> result;
    for (const auto& binding : commandToKeys) {
        CommandType cmd = binding.first;
        const std::vector<std::string>& keys = binding.second;
        result.emplace_back(CommandUtils::commandTypeToString(cmd), keys);
    }
    
    std::sort(result.begin(), result.end(), 
              [](const auto& a, const auto& b) { return a.first < b.first; });
    
    return result;
}

std::string InputHandler::getControlsHelp() const {
    // Определяем клавиши для движения
    std::string moveHelp;
    std::vector<char> moveKeys;
    
    std::vector<CommandType> moveCommands = {
        CommandType::MOVE_UP, CommandType::MOVE_LEFT, 
        CommandType::MOVE_DOWN, CommandType::MOVE_RIGHT
    };
    
    for (const auto& cmd : moveCommands) {
        char key = findKeyForCommand(cmd);
        if (key != '\0') {
            moveKeys.push_back(key);
        }
    }
    
    if (moveKeys.size() == 4) {
        moveHelp = std::string(1, moveKeys[0]) + std::string(1, moveKeys[1]) + 
                  std::string(1, moveKeys[2]) + std::string(1, moveKeys[3]) + "-move";
    } else {
        moveHelp = "WASD-move (custom)";
    }
    
    std::vector<std::string> helps = {moveHelp};
    
    // Добавляем остальные команды
    std::vector<std::pair<CommandType, std::string>> commandHelpPairs = {
        {CommandType::CAST_SPELL, "spell"},
        {CommandType::SWITCH_COMBAT, "combat"},
        {CommandType::BUY_SPELL, "buy"},
        {CommandType::SAVE_GAME, "save"},
        {CommandType::SHOW_MENU, "menu"},
        {CommandType::QUIT_GAME, "quit"}
    };
    
    for (const auto& pair : commandHelpPairs) {
        CommandType cmd = pair.first;
        const std::string& helpText = pair.second;
        char key = findKeyForCommand(cmd);
        if (key != '\0') {
            helps.push_back(std::string(1, key) + "-" + helpText);
        }
    }
    

    std::string result;
    for (size_t i = 0; i < helps.size(); ++i) {
        result += helps[i];
        if (i < helps.size() - 1) {
            result += ", ";
        }
    }
    
    return result;
}

InputHandler::RebindResult InputHandler::rebindKey(CommandType command, char newKey) {
    RebindResult result;
    result.commandName = CommandUtils::commandTypeToString(command);
    result.newKey = newKey;
    
    result.oldKey = findKeyForCommand(command);
    
    auto it = keyBindings.find(newKey);
    if (it != keyBindings.end() && it->second != command) {
        result.success = false;
        result.message = "Key '" + std::string(1, newKey) + 
                       "' already assigned to '" + 
                       CommandUtils::commandTypeToString(it->second) + "'";
        return result;
    }
    
    for (auto it = keyBindings.begin(); it != keyBindings.end(); ) {
        if (it->second == command) {
            it = keyBindings.erase(it);
        } else {
            ++it;
        }
    }
    
    keyBindings[newKey] = command;
    
    if (std::isalpha(newKey)) {
        keyBindings[std::toupper(newKey)] = command;
    }
    
    saveBindingsToFile(keyBindings);
    
    result.success = true;
    result.message = "Successfully rebound '" + result.commandName + 
                    "' from '" + std::string(1, result.oldKey) + 
                    "' to '" + std::string(1, newKey) + "'";
    return result;
}

bool InputHandler::resetToDefaults() {
    saveBindingsToFile(defaultKeyBindings);
    keyBindings = defaultKeyBindings;
    return true;
}

std::vector<std::pair<CommandType, std::string>> InputHandler::getCommandsForRebinding() const {
    std::vector<std::pair<CommandType, std::string>> commands;
    
    for (const auto& cmd : CommandUtils::getAllCommandTypes()) {
        commands.emplace_back(cmd, getCommandDescription(cmd));
    }
    
    return commands;
}

char InputHandler::getKeyForCommand(CommandType command) const {
    return findKeyForCommand(command);
}