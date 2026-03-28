#include "../include/KeyBindingConfig.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <set>

KeyBindingConfig::KeyBindingConfig() : isLoaded(false) {
    setDefaultBindings();
}

KeyBindingConfig& KeyBindingConfig::getInstance() {
    static KeyBindingConfig instance;
    return instance;
}

void KeyBindingConfig::setDefaultBindings() {
    keyToCommand.clear();
    keyToDirection.clear();
    commandToKey.clear();
    
    keyToDirection['w'] = Direction::UP;
    keyToDirection['s'] = Direction::DOWN;
    keyToDirection['a'] = Direction::LEFT;
    keyToDirection['d'] = Direction::RIGHT;
    
    keyToCommand['w'] = CommandType::MOVE;
    keyToCommand['s'] = CommandType::MOVE;
    keyToCommand['a'] = CommandType::MOVE;
    keyToCommand['d'] = CommandType::MOVE;
    
    keyToCommand['x'] = CommandType::ATTACK;
    keyToCommand['m'] = CommandType::SWITCH_MODE;
    keyToCommand[' '] = CommandType::SKIP_TURN;
    keyToCommand['c'] = CommandType::CAST_SPELL;
    keyToCommand['b'] = CommandType::BUY_SPELL;
    keyToCommand['r'] = CommandType::RESTART;
    keyToCommand['q'] = CommandType::QUIT;
    
    commandToKey[CommandType::ATTACK] = 'x';
    commandToKey[CommandType::SWITCH_MODE] = 'm';
    commandToKey[CommandType::SKIP_TURN] = ' ';
    commandToKey[CommandType::CAST_SPELL] = 'c';
    commandToKey[CommandType::BUY_SPELL] = 'b';
    commandToKey[CommandType::RESTART] = 'r';
    commandToKey[CommandType::QUIT] = 'q';
    
    isLoaded = true;
}

void KeyBindingConfig::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw KeyBindingException("Cannot open keybinding file: " + filename);
    }
    
    keyToCommand.clear();
    keyToDirection.clear();
    commandToKey.clear();
    
    std::set<char> usedKeys;
    std::set<CommandType> definedCommands;
    std::string line;
    int lineNumber = 0;
    
    while (std::getline(file, line)) {
        lineNumber++;
        
        if (line.empty() || line[0] == '#') continue;
        
        std::istringstream iss(line);
        std::string keyStr, commandStr, dirStr;
        
        if (!(iss >> keyStr >> commandStr)) {
            continue;
        }
        
        if (keyStr.length() != 1) {
            throw KeyBindingException("Invalid key at line " + std::to_string(lineNumber) + 
                                     ": key must be single character");
        }
        
        char key = keyStr[0];
        
        if (usedKeys.find(key) != usedKeys.end()) {
            throw KeyBindingException("Duplicate key binding at line " + std::to_string(lineNumber) + 
                                     ": key '" + keyStr + "' already assigned");
        }
        
        CommandType cmdType;
        if (!parseCommandType(commandStr, cmdType)) {
            throw KeyBindingException("Unknown command at line " + std::to_string(lineNumber) + 
                                     ": " + commandStr);
        }
        
        if (cmdType == CommandType::MOVE) {
            if (!(iss >> dirStr)) {
                throw KeyBindingException("Direction required for MOVE at line " + 
                                         std::to_string(lineNumber));
            }
            Direction dir;
            if (!parseDirection(dirStr, dir)) {
                throw KeyBindingException("Invalid direction at line " + std::to_string(lineNumber) + 
                                         ": " + dirStr);
            }
            keyToDirection[key] = dir;
        }
        
        usedKeys.insert(key);
        keyToCommand[key] = cmdType;
        
        if (cmdType != CommandType::MOVE) {
            if (definedCommands.find(cmdType) != definedCommands.end()) {
                throw KeyBindingException("Multiple keys for command at line " + 
                                         std::to_string(lineNumber));
            }
            definedCommands.insert(cmdType);
            commandToKey[cmdType] = key;
        }
    }
    
    validateBindings();
    isLoaded = true;
}

void KeyBindingConfig::validateBindings() const {
    std::vector<CommandType> requiredCommands = {
        CommandType::ATTACK,
        CommandType::SWITCH_MODE,
        CommandType::SKIP_TURN
    };
    
    for (CommandType cmd : requiredCommands) {
        if (commandToKey.find(cmd) == commandToKey.end()) {
            std::string cmdName;
            switch (cmd) {
                case CommandType::ATTACK: cmdName = "ATTACK"; break;
                case CommandType::SWITCH_MODE: cmdName = "SWITCH_MODE"; break;
                case CommandType::SKIP_TURN: cmdName = "SKIP_TURN"; break;
                default: cmdName = "UNKNOWN"; break;
            }
            throw KeyBindingException("Required command not bound: " + cmdName);
        }
    }
    
    bool hasUp = false, hasDown = false, hasLeft = false, hasRight = false;
    for (const auto& pair : keyToDirection) {
        switch (pair.second) {
            case Direction::UP: hasUp = true; break;
            case Direction::DOWN: hasDown = true; break;
            case Direction::LEFT: hasLeft = true; break;
            case Direction::RIGHT: hasRight = true; break;
            default: break;
        }
    }
    
    if (!hasUp || !hasDown || !hasLeft || !hasRight) {
        throw KeyBindingException("All four directions must be bound");
    }
}

void KeyBindingConfig::saveToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw KeyBindingException("Cannot create keybinding file: " + filename);
    }
    
    file << "# Key Bindings Configuration\n";
    file << "# Format: KEY COMMAND [DIRECTION]\n";
    file << "# Commands: MOVE, ATTACK, SWITCH_MODE, SKIP_TURN, CAST_SPELL, BUY_SPELL, RESTART, QUIT\n";
    file << "# Directions: UP, DOWN, LEFT, RIGHT\n\n";
    
    for (const auto& pair : keyToDirection) {
        std::string dirStr;
        switch (pair.second) {
            case Direction::UP: dirStr = "UP"; break;
            case Direction::DOWN: dirStr = "DOWN"; break;
            case Direction::LEFT: dirStr = "LEFT"; break;
            case Direction::RIGHT: dirStr = "RIGHT"; break;
            default: continue;
        }
        file << pair.first << " MOVE " << dirStr << "\n";
    }
    
    for (const auto& pair : commandToKey) {
        std::string cmdStr;
        switch (pair.first) {
            case CommandType::ATTACK: cmdStr = "ATTACK"; break;
            case CommandType::SWITCH_MODE: cmdStr = "SWITCH_MODE"; break;
            case CommandType::SKIP_TURN: cmdStr = "SKIP_TURN"; break;
            case CommandType::CAST_SPELL: cmdStr = "CAST_SPELL"; break;
            case CommandType::BUY_SPELL: cmdStr = "BUY_SPELL"; break;
            case CommandType::RESTART: cmdStr = "RESTART"; break;
            case CommandType::QUIT: cmdStr = "QUIT"; break;
            default: continue;
        }
        file << pair.second << " " << cmdStr << "\n";
    }
}

bool KeyBindingConfig::parseCommandType(const std::string& commandStr, CommandType& outType) const {
    std::string upper = commandStr;
    std::transform(upper.begin(), upper.end(), upper.begin(), ::toupper);
    
    if (upper == "MOVE") { outType = CommandType::MOVE; return true; }
    if (upper == "ATTACK") { outType = CommandType::ATTACK; return true; }
    if (upper == "SWITCH_MODE") { outType = CommandType::SWITCH_MODE; return true; }
    if (upper == "SKIP_TURN") { outType = CommandType::SKIP_TURN; return true; }
    if (upper == "CAST_SPELL") { outType = CommandType::CAST_SPELL; return true; }
    if (upper == "BUY_SPELL") { outType = CommandType::BUY_SPELL; return true; }
    if (upper == "SAVE_GAME") { outType = CommandType::SAVE_GAME; return true; }
    if (upper == "RESTART") { outType = CommandType::RESTART; return true; }
    if (upper == "QUIT") { outType = CommandType::QUIT; return true; }
    
    return false;
}

bool KeyBindingConfig::parseDirection(const std::string& dirStr, Direction& outDir) const {
    std::string upper = dirStr;
    std::transform(upper.begin(), upper.end(), upper.begin(), ::toupper);
    
    if (upper == "UP") { outDir = Direction::UP; return true; }
    if (upper == "DOWN") { outDir = Direction::DOWN; return true; }
    if (upper == "LEFT") { outDir = Direction::LEFT; return true; }
    if (upper == "RIGHT") { outDir = Direction::RIGHT; return true; }
    
    return false;
}

CommandType KeyBindingConfig::getCommandForKey(char key) const {
    auto it = keyToCommand.find(key);
    if (it != keyToCommand.end()) {
        return it->second;
    }
    return CommandType::INVALID;
}

Direction KeyBindingConfig::getDirectionForKey(char key) const {
    auto it = keyToDirection.find(key);
    if (it != keyToDirection.end()) {
        return it->second;
    }
    return Direction::NONE;
}

char KeyBindingConfig::getKeyForCommand(CommandType type) const {
    auto it = commandToKey.find(type);
    if (it != commandToKey.end()) {
        return it->second;
    }
    return '\0';
}

bool KeyBindingConfig::hasBinding(char key) const {
    return keyToCommand.find(key) != keyToCommand.end();
}

bool KeyBindingConfig::isDirectionKey(char key) const {
    return keyToDirection.find(key) != keyToDirection.end();
}

std::vector<std::pair<char, std::string>> KeyBindingConfig::getAllBindings() const {
    std::vector<std::pair<char, std::string>> result;
    
    for (const auto& pair : keyToCommand) {
        std::string desc;
        switch (pair.second) {
            case CommandType::MOVE: {
                auto dirIt = keyToDirection.find(pair.first);
                if (dirIt != keyToDirection.end()) {
                    switch (dirIt->second) {
                        case Direction::UP: desc = "Move Up"; break;
                        case Direction::DOWN: desc = "Move Down"; break;
                        case Direction::LEFT: desc = "Move Left"; break;
                        case Direction::RIGHT: desc = "Move Right"; break;
                        default: desc = "Move"; break;
                    }
                }
                break;
            }
            case CommandType::ATTACK: desc = "Attack"; break;
            case CommandType::SWITCH_MODE: desc = "Switch Mode"; break;
            case CommandType::SKIP_TURN: desc = "Skip Turn"; break;
            case CommandType::CAST_SPELL: desc = "Cast Spell"; break;
            case CommandType::BUY_SPELL: desc = "Buy Spell"; break;
            case CommandType::SAVE_GAME: desc = "Save Game"; break;
            case CommandType::RESTART: desc = "Restart"; break;
            case CommandType::QUIT: desc = "Quit"; break;
            default: desc = "Unknown"; break;
        }
        result.push_back({pair.first, desc});
    }
    
    return result;
}
