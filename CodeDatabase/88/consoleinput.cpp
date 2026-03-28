#include "consoleinput.hpp"

std::unique_ptr<Command> ConsoleInputHandler::parseCommand(const std::string& input) {
    std::string lowerInput = input;
    std::transform(lowerInput.begin(), lowerInput.end(), lowerInput.begin(), 
                   [](unsigned char c) { return std::tolower(c); });
    
    if (lowerInput.rfind("cast ", 0) == 0) {
        try {
            int spellIndex = std::stoi(lowerInput.substr(5));
            return std::make_unique<CastSpellCommand>(spellIndex);
        } catch (...) {
            return nullptr; 
        }
    }
    
    auto it = commandMap.find(lowerInput);
    if (it != commandMap.end()) {
        std::string commandName = it->second;
        
        std::map<std::string, std::string> commandToDirection = {
            {"move_up", "w"},
            {"move_left", "a"},
            {"move_down", "s"},
            {"move_right", "d"}
        };
        
        auto dirIt = commandToDirection.find(commandName);
        if (dirIt != commandToDirection.end()) {
            return std::make_unique<MoveCommand>(dirIt->second);
        } else if (commandName == "switch_weapon") {
            return std::make_unique<SwitchWeaponCommand>();
        } else if (commandName == "end_game") {
            return std::make_unique<EndGameCommand>();
        } else if (commandName == "save_game") {
            return std::make_unique<SaveGameCommand>();
        } else if (commandName == "help") {
            return std::make_unique<HelpCommand>();
        }
    }
    
    return nullptr; 
}

void ConsoleInputHandler::printAvailableCommands() const {
    std::cout << "Available commands:\n";
    for (const auto& [key, command] : commandMap) {
        std::cout << "  " << key << " -> " << command << "\n";
    }
    std::cout << "  cast <number> -> cast spell\n";
}

bool ConsoleInputHandler::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Cannot open config file: " << filename << "\n";
        return false;
    }
    
    try {
        json j;
        file >> j;
        
        if (!j.contains("commands")) {
            std::cerr << "Invalid config file format. Missing 'commands' section.\n";
            return false;
        }
        
        std::vector<std::string> requiredCommands = {
            "move_up", "move_left", "move_down", "move_right",
            "switch_weapon", "end_game", "save_game", "help"
        };
        
        std::map<std::string, std::string> tempMap;
        std::set<std::string> foundCommands; 
        bool hasError = false;
        
        for (auto& [key, value] : j["commands"].items()) {
            std::string command = value.get<std::string>();
            
            if (tempMap.find(key) != tempMap.end()) {
                std::cerr << "Error: Key '" << key << "' has multiple commands.\n";
                hasError = true;
                break;
            }
            
            tempMap[key] = command;
            foundCommands.insert(command);
        }
        
        if (!hasError) {
            for (const auto& requiredCmd : requiredCommands) {
                if (foundCommands.find(requiredCmd) == foundCommands.end()) {
                    std::cerr << "Error: Missing required command '" << requiredCmd << "' in config file.\n";
                    hasError = true;
                    break;
                }
            }
        }
        
        if (!hasError) {
            commandMap = tempMap;
            std::cout << "Commands successfully loaded from: " << filename << "\n";
            return true;
        } else {
            std::cerr << "Configuration file has errors. Using default commands.\n";
        }
    } catch (const std::exception& e) {
        std::cerr << "Error reading config file: " << e.what() << "\n";
    }
    
    return false;
}

const std::map<std::string, std::string>& ConsoleInputHandler::getCommandMap() const {
    return commandMap;
}

void ConsoleInputHandler::loadDefaultCommands() {
    commandMap = {
        {"w", "move_up"},
        {"a", "move_left"},
        {"s", "move_down"},
        {"d", "move_right"},
        {"switch", "switch_weapon"},
        {"end", "end_game"},
        {"quit", "end_game"},
        {"save", "save_game"},
        {"help", "help"}
    };
}